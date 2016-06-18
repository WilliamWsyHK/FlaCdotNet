using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using FlaCdotNet;
using FlaCdotNet.Encoder;
using FlaCdotNet.Metadata;

namespace FlaCdotNetEncoderTest
{
    class OurEncoder
        : FlaCdotNet.Encoder.File
    {
        public OurEncoder()
            : base()
        {
        }

        protected override void progressCallback(ulong bytesWritten, ulong samplesWritten, uint framesWritten, uint totalFramesEstimate)
        {
            Console.Error.WriteLine($"wrote {bytesWritten} bytes, {samplesWritten}/{Constants.totalSamples}, {framesWritten}/{totalFramesEstimate}");
        }
    }

    internal static class Constants
    {
        internal const int ReadSize = 1024;
        internal static uint totalSamples = 0;

        /// <summary>
        /// Samples * BytesPerSample * Channels
        /// </summary>
        internal static byte[] buffer = new byte[ReadSize * 2 * 2];

        /// <summary>
        /// Samples * Channels
        /// </summary>
        internal static int[] pcm = new int[ReadSize * 2 * 2];
    }

    class Program
    {
        static void Main(string[] args)
        {
            bool ok = true;
            OurEncoder encoder;
            InitStatus initStatus;
            Prototype[] metadata = new Prototype[2];
            VorbisComment.Entry artistEntry = new VorbisComment.Entry(),
                yearEntry = new VorbisComment.Entry();
            BinaryReader br = null;
            uint sampleRate = 0u;
            uint channels = 0u;
            uint bps = 0u;

            string[] argv = { "", "Test中文名(Chinese Name).wav", "Whatever-char-encoding.flac" };

            try
            {
                br = new BinaryReader(System.IO.File.Open(argv[1], FileMode.Open));
            }
            catch (UnauthorizedAccessException e)
            {
                Console.Error.WriteLine(e);
                Environment.Exit(1);
                return;
            }
            catch (FileNotFoundException)
            {
                Console.Error.WriteLine($"{argv[1]} not found in the same directory of this program!");
                Environment.Exit(1);
                return;
            }
            finally
            {
                if (br == null)
                {
                    Console.Error.WriteLine($"Error opening {argv[1]} for input");
                    Environment.Exit(1);
                }
            }

            encoder = new OurEncoder();

            if ((Constants.buffer = br.ReadBytes(44)).Length != 44
                || !Constants.buffer.Take(4).SequenceEqual(Encoding.ASCII.GetBytes("RIFF"))
                || !Constants.buffer.Skip(8).Take(16).SequenceEqual(Encoding.ASCII.GetBytes("WAVEfmt \x10\x00\x00\x00\x01\x00\x02\x00"))
                || !Constants.buffer.Skip(32).Take(8).SequenceEqual(Encoding.ASCII.GetBytes("\x04\x00\x10\x00" + "data")))
            {
                Console.Error.WriteLine("ERROR: invalid/unsupported WAVE file, only 16bps stereo WAVE in canonical form allowed");
                br.Dispose();
                Environment.Exit(1);
                return;
            }

            // little-endian machine assumed
            sampleRate = ((((((uint)Constants.buffer[27] << 8) | Constants.buffer[26]) << 8) | Constants.buffer[25]) << 8) | Constants.buffer[24];
            channels = 2;
            bps = 16;
            Constants.totalSamples = (((((((uint)Constants.buffer[43] << 8) | Constants.buffer[42]) << 8) | Constants.buffer[41]) << 8) | Constants.buffer[40]) / 4;

            if (!(encoder?.IsValid()).Value)
            {
                Console.Error.WriteLine("ERROR: allocating encoder");
                br.Dispose();
                Environment.Exit(1);
                return;
            }

            ok &= encoder.SetVerify(true);
            ok &= encoder.SetCompressionLevel(5u);
            ok &= encoder.SetChannels(channels);
            ok &= encoder.SetBitsPerSample(bps);
            ok &= encoder.SetSampleRate(sampleRate);
            ok &= encoder.SetTotalSamplesEstimate(Constants.totalSamples);

            if (ok)
            {
                if (
                    (metadata[0] = new VorbisComment()) == null ||
                    (metadata[1] = new Padding()) == null ||
                    !(artistEntry.SetFieldName("ARTIST") && artistEntry.SetFieldValue("威廉(William)")) ||
                    !(metadata[0] as VorbisComment).AppendComment(artistEntry) ||
                    !(yearEntry.SetFieldName("YEAR") && yearEntry.SetFieldValue("1984")) ||
                    !(metadata[0] as VorbisComment).AppendComment(yearEntry)
                )
                {
                    Console.Error.WriteLine("ERROR: out of memory or tag error");
                    ok = false;
                }

                (metadata[1] as Padding).Length = 1234;
                ok = encoder.SetMetadata(metadata);
            }

            if (ok)
            {
                initStatus = encoder.Init(argv[2]);
                if (initStatus != InitStatus.Ok)
                {
                    Console.Error.WriteLine($"ERROR: initializing encoder: {initStatus.ToString()}");
                    ok = false;
                }
            }

            if (ok)
            {
                uint left = Constants.totalSamples;
                while (ok && left != 0)
                {
                    uint need = (left > Constants.ReadSize ? Constants.ReadSize : left);
                    if ((Constants.buffer = br.ReadBytes((int)(channels * (bps / 8) * need))).Length != (channels * (bps / 8) * need))
                    {
                        Console.Error.WriteLine("ERROR: reading from WAVE file");
                        ok = false;
                    }
                    else
                    {
                        for (var i = 0; i < need * channels; i++)
                        {
                            Constants.pcm[i] = ((sbyte)Constants.buffer[2 * i + 1] << 8) | Constants.buffer[2 * i];
                        }
                        ok = encoder.ProcessInterleaved(Constants.pcm, need);
                    }
                    left -= need;
                }
            }

            ok &= encoder.Finish();

            Console.Error.WriteLine($"encoding: {(ok ? "succeeded" : "FAILED")}");
            Console.Error.WriteLine($"   state: {encoder.GetState().ToString()}");

            artistEntry?.Dispose();
            yearEntry?.Dispose();
            foreach (var m in metadata)
            {
                m?.Dispose();
            }
            encoder?.Dispose();
            br?.Dispose();
        }
    }
}
