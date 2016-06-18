using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using FlaCdotNet;
using FlaCdotNet.Decoder;
using FlaCdotNet.Metadata;

namespace FlaCdotNetDecoderTest
{
    internal static class Constants
    {
        internal static ulong totalSamples = 0;
        internal static uint sampleRate = 0;
        internal static uint channels = 0;
        internal static uint bps = 0;

        internal static void writeLittleEndianUShort(BinaryWriter bw, ushort x)
        {
            bw.Write(x);
        }

        internal static void writeLittleEndianShort(BinaryWriter bw, short x)
        {
            bw.Write(x);
        }

        internal static void writeLittleEndianUInt(BinaryWriter bw, uint x)
        {
            bw.Write(x);
        }
    }

    class OurDecoder
        : FlaCdotNet.Decoder.File
    {
        protected BinaryWriter bw_;

        public OurDecoder(BinaryWriter bw)
            : base()
        {
            bw_ = bw;
        }

        protected override WriteStatus writeCallback(Frame frame, int[,] buffer)
        {
            uint totalSize = (uint)(Constants.totalSamples * Constants.channels * (Constants.bps / 8));
            if (Constants.totalSamples == 0)
            {
                Console.Error.WriteLine("ERROR: this example only works for FLAC files that have a total_samples count in STREAMINFO");
                return WriteStatus.Abort;
            }

            if (Constants.channels != 2 || Constants.bps != 16)
            {
                Console.Error.WriteLine("ERROR: this example only supports 16bit stereo streams");
                return WriteStatus.Abort;
            }

            if (frame.Header.SampleNumber == 0)
            {
                try
                {
                    bw_.Write(Encoding.ASCII.GetBytes("RIFF"));
                    Constants.writeLittleEndianUInt(bw_, totalSize + 36);
                    bw_.Write(Encoding.ASCII.GetBytes("WAVEfmt "));
                    Constants.writeLittleEndianUInt(bw_, 16);
                    Constants.writeLittleEndianUShort(bw_, 1);
                    Constants.writeLittleEndianUShort(bw_, (ushort)Constants.channels);
                    Constants.writeLittleEndianUInt(bw_, Constants.sampleRate);
                    Constants.writeLittleEndianUInt(bw_, Constants.sampleRate * Constants.channels * (Constants.bps / 8));
                    Constants.writeLittleEndianUShort(bw_, (ushort)(Constants.channels * (Constants.bps / 8)));
                    Constants.writeLittleEndianUShort(bw_, (ushort)Constants.bps);
                    bw_.Write(Encoding.ASCII.GetBytes("data"));
                    Constants.writeLittleEndianUInt(bw_, totalSize);
                }
                catch (IOException e)
                {
                    Console.Error.WriteLine("ERROR: write error");
                    Console.Error.WriteLine(e);
                    return WriteStatus.Abort;
                }
            }
            
            for (uint i = 0u; i < frame.Header.BlockSize; i++)
            {
                short leftChannel = (short)buffer[0, i];
                short rightChannel = (short)buffer[1, i];
                try
                {
                    Constants.writeLittleEndianShort(bw_, leftChannel);
                    Constants.writeLittleEndianShort(bw_, rightChannel);
                }
                catch (IOException e)
                {
                    Console.Error.WriteLine("ERROR: write error");
                    Console.Error.WriteLine(e);
                    return WriteStatus.Abort;
                }
            }

            return WriteStatus.Continue;
        }

        protected override void metadataCallback(Prototype metadata)
        {
            if (metadata.MetadataType == MetadataType.StreamInfo)
            {
                var m = metadata as StreamInfo;

                Constants.totalSamples = m.TotalSamples;
                Constants.sampleRate = m.SampleRate;
                Constants.channels = m.Channels;
                Constants.bps = m.BitsPerSample;

                Console.Error.WriteLine($"sample rate    : {Constants.sampleRate} Hz");
                Console.Error.WriteLine($"channels       : {Constants.channels}");
                Console.Error.WriteLine($"bits per sample: {Constants.bps}");
                Console.Error.WriteLine($"total samples  : {Constants.totalSamples}");
            }
        }

        protected override void errorCallback(ErrorStatus status)
        {
            Console.Error.WriteLine($"Got error callback: {status.ToString()}");
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            bool ok = true;
            BinaryWriter bw = null;

            //string[] argv = { "", "abc.flac", "Whatever-char-encoding.wav" };
            string[] argv = { "", "Test中文名(Chinese Name).flac", "Whatever-char-encoding.wav" };

            try
            {
                bw = new BinaryWriter(System.IO.File.Open(argv[2], FileMode.Create), Encoding.ASCII);
            }
            catch (UnauthorizedAccessException e)
            {
                Console.Error.WriteLine(e);
                Environment.Exit(1);
                return;
            }
            finally
            {
                if (bw == null)
                {
                    Console.Error.WriteLine($"ERROR: opening {argv[2]} for output");
                    Environment.Exit(1);
                }
            }

            OurDecoder decoder = new OurDecoder(bw);

            if (!(decoder?.IsValid()).Value)
            {
                Console.Error.WriteLine("ERROR: allocating decoder");
                Environment.Exit(1);
                return;
            }

            decoder.SetMD5Checking(true);

            if (!System.IO.File.Exists(argv[1]))
            {
                Console.Error.WriteLine($"{argv[1]} not found in the same directory of this program!");
                Environment.Exit(1);
                return;
            }

            InitStatus initStatus = decoder.Init(argv[1]);
            if (initStatus != InitStatus.Ok)
            {
                Console.Error.WriteLine($"ERROR: initializing decoder: {initStatus.ToString()}");
                ok = false;
            }

            if (ok)
            {
                ok = decoder.ProcessUntilEndOfStream();
                Console.Error.WriteLine($"decoding: {(ok ? "succeeded" : "FAILED")}");
                Console.Error.WriteLine($"   state: {decoder.GetState().ToString()}");
            }

            decoder.Dispose();
            bw.Dispose();
        }
    }
}
