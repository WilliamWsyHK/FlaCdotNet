using System;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using FlaCdotNet;
using FlaCdotNet.Encoder;
using FlaCdotNet.Metadata;

namespace UnitTest_FlaCdotNet
{
    [TestClass]
    public class EncoderUnitTest
    {
        public enum Layer
        {
            Stream = 0,
            SeekableStream,
            File,
            FileName
        }

        public static string[] LayerString =
        {
            "Stream",
            "Seekable Stream",
            "FILE*",
            "Filename"
        };

        static StreamInfo streamInfo_ = new StreamInfo();
        static Padding padding_ = new Padding();
        static SeekTable seekTable_ = new SeekTable();
        static Application application1_ = new Application();
        static Application application2_ = new Application();
        static VorbisComment vorbisComment_ = new VorbisComment();
        static CueSheet cueSheet_ = new CueSheet();
        static Picture picture_ = new Picture();
        static /*Unknown*/Application unknown_ = new /*Unknown*/Application();
        static Prototype[] metadataSequence_ = new Prototype[] { vorbisComment_, padding_, seekTable_, application1_, application2_, cueSheet_, picture_, unknown_ };
        //static Prototype[] metadataSequence_ = new Prototype[] { vorbisComment_, padding_, seekTable_, cueSheet_, picture_, unknown_ };
        //static Prototype[] metadataSequence_ = new Prototype[] { picture_, unknown_ };

        static string flacFileName(bool isOgg)
        {
            return isOgg ? "metadata.oga" : "metadata.flac";
        }

        static bool die_([Pure]string msg)
        {
            Console.Error.WriteLine($"ERROR: {msg}");
            return false;
        }

        static bool die_s_([Pure]string msg, [Pure]FlaCdotNet.Encoder.Stream encoder)
        {
            var state = encoder.GetState();

            if (!string.IsNullOrEmpty(msg))
            {
                Console.Error.Write($"FAILED: {msg}");
            }
            else
            {
                Console.Error.Write("FAILED");
            }
            Console.Error.WriteLine($", state = {(int)(State)state} ({state.ToString()})");
            if ((State)state == State.VerifyDecoderError)
            {
                var dstate = encoder.GetVerifyDecoderState();
                Console.Error.WriteLine($"\tverify decoder state = {(uint)(FlaCdotNet.Decoder.State)dstate} ({dstate.ToString()})");
            }
            return false;
        }

        static void initMetadataBlocks()
        {
            TestCommon.multis_init_metadata_blocks(streamInfo_, padding_, seekTable_, application1_, application2_, vorbisComment_, cueSheet_, picture_, unknown_);
        }

        static void freeMetadataBlocks()
        {
            TestCommon.mutils__free_metadata_blocks(streamInfo_, padding_, seekTable_, application1_, application2_, vorbisComment_, cueSheet_, picture_, unknown_);
        }

        public class StreamEncoder
            : FlaCdotNet.Encoder.Stream
        {
            public Layer layer_;
            public FileStream file_;

            public StreamEncoder(Layer layer)
                : base()
            {
                layer_ = layer;
            }

            protected override ReadStatus readCallback(byte[] buffer, ref uint bytes)
            {
                if (!file_.CanRead)
                {
                    return ReadStatus.Unsupported;
                }

                if (bytes > 0)
                {
                    try
                    {
                        bytes = (uint)file_.Read(buffer, 0, (int)bytes);
                        if (bytes == 0)
                        {
                            return ReadStatus.EndOfStream;
                        }
                    }
                    catch (Exception e) when (!(e is ArgumentNullException) && !(e is ArgumentOutOfRangeException))
                    {
                        return ReadStatus.Abort;
                    }
                    return ReadStatus.Continue;
                }
                else
                {
                    return ReadStatus.Abort;
                }
            }

            protected override WriteStatus writeCallback([Pure] byte[] buffer, uint bytes, uint samples, uint currentFrame)
            {
                try
                {
                    file_.Write(buffer, 0, (int)bytes);
                    return WriteStatus.Ok;
                }
                catch (Exception e) when (!(e is ArgumentNullException) && !(e is ArgumentOutOfRangeException))
                {
                    return WriteStatus.FatalError;
                }
            }

            protected override SeekStatus seekCallback(ulong absoluteByteOffset)
            {
                if (layer_ == Layer.Stream)
                {
                    return SeekStatus.Unsupported;
                }
                else if (file_.Seek((long)absoluteByteOffset, SeekOrigin.Begin) < 0)
                {
                    return SeekStatus.Error;
                }
                else
                {
                    return SeekStatus.Ok;
                }
            }

            protected override TellStatus tellCallback(ref ulong absoluteByteOffset)
            {
                if (layer_ == Layer.Stream)
                {
                    return TellStatus.Unsupported;
                }

                var offset = file_.Position;

                if (offset < 0)
                {
                    return TellStatus.Error;
                }

                absoluteByteOffset = (ulong)offset;
                return TellStatus.Ok;
            }

            protected override void metadataCallback([Pure] Prototype metadata)
            {
            }
        }

        public class FileEncoder
            : FlaCdotNet.Encoder.File
        {
            public Layer layer_;

            public FileEncoder(Layer layer)
                : base()
            {
                layer_ = layer;
            }

            protected override void progressCallback(ulong bytesWritten, ulong samplesWritten, uint framesWritten, uint totalFramesEstimate)
            {
            }
        }

        static FlaCdotNet.Encoder.Stream NewByLayer(Layer layer)
        {
            if (layer < Layer.File)
            {
                return new StreamEncoder(layer);
            }
            else
            {
                return new FileEncoder(layer);
            }
        }

        static bool TestStreamEncoder(Layer layer, bool isOgg)
        {
            FlaCdotNet.Encoder.Stream encoder;
            FlaCdotNet.Encoder.InitStatus initStatus;
            FileStream file = null;
            var samples = new int[1024];
            //var samplesArray = new int[][] { samples };
            var samplesArray = new int[1, 1024];

            Console.WriteLine($"{Environment.NewLine}+++ {nameof(FlaCdotNet)} unit test: {(layer < Layer.File ? nameof(FlaCdotNet.Encoder.Stream) : nameof(FlaCdotNet.Encoder.File))} (layer: {LayerString[(int)layer]}, format: {(isOgg ? "Ogg FLAC" : "FLAC")}){Environment.NewLine}");

            Console.Write($"allocating {nameof(encoder)} instance... ");
            encoder = NewByLayer(layer);
            if (encoder == null)
            {
                Console.Error.WriteLine("FAILED, new returned NULL");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.IsValid)}... ");
            if (!encoder.IsValid())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            if (isOgg)
            {
                Console.Write($"testing {nameof(encoder.SetOggSerialNumber)}... ");
                if (!encoder.SetOggSerialNumber(TestCommon.file_utils__ogg_serial_number))
                {
                    return die_s_("returned false", encoder);
                }
                Console.WriteLine("OK");
            }

            Console.Write($"testing {nameof(encoder.SetVerify)}... ");
            if (!encoder.SetVerify(true))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetStreamableSubset)}... ");
            if (!encoder.SetStreamableSubset(true))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetChannels)}... ");
            if (!encoder.SetChannels(streamInfo_.Channels))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetBitsPerSample)}... ");
            if (!encoder.SetBitsPerSample(streamInfo_.BitsPerSample))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetSampleRate)}... ");
            if (!encoder.SetSampleRate(streamInfo_.SampleRate))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetCompressionLevel)}... ");
            if (!encoder.SetCompressionLevel(uint.MaxValue))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetBlockSize)}... ");
            if (!encoder.SetBlockSize(streamInfo_.MinBlockSize))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetDoMidSideStereo)}... ");
            if (!encoder.SetDoMidSideStereo(false))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetLooseMidSideStereo)}... ");
            if (!encoder.SetLooseMidSideStereo(false))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetMaxLpcOrder)}... ");
            if (!encoder.SetMaxLpcOrder(0u))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetQlpCoeffPrecision)}... ");
            if (!encoder.SetQlpCoeffPrecision(0u))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetDoQlpCoeffPrecSearch)}... ");
            if (!encoder.SetDoQlpCoeffPrecSearch(false))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetDoEscapeCoding)}... ");
            if (!encoder.SetDoEscapeCoding(false))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetDoExhaustiveModelSearch)}... ");
            if (!encoder.SetDoExhaustiveModelSearch(false))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetMinResidualPartitionOrder)}... ");
            if (!encoder.SetMinResidualPartitionOrder(0u))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetMaxResidualPartitionOrder)}... ");
            if (!encoder.SetMaxResidualPartitionOrder(0u))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetRiceParameterSearchDist)}... ");
            if (!encoder.SetRiceParameterSearchDist(0u))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetTotalSamplesEstimate)}... ");
            if (!encoder.SetTotalSamplesEstimate(streamInfo_.TotalSamples))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.SetMetadata)}... ");
            if (!encoder.SetMetadata(metadataSequence_))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            if (layer < Layer.FileName)
            {
                Console.Write("opening file for FLAC output... ");
                try
                {
                    file = System.IO.File.Open(flacFileName(isOgg), FileMode.OpenOrCreate, FileAccess.ReadWrite);
                }
                catch (Exception e) when (!(e is ArgumentException) && !(e is ArgumentNullException))
                {
                    Console.Error.WriteLine($"ERROR ({e.Message})");
                    return false;
                }
                Console.WriteLine("OK");
                if (layer < Layer.File)
                {
                    (encoder as StreamEncoder).file_ = file;
                }
            }

            switch (layer)
            {
                case Layer.Stream:
                    goto case Layer.SeekableStream;
                case Layer.SeekableStream:
                    Console.Write($"testing {(isOgg ? nameof(encoder.InitOgg) : nameof(encoder.Init))}... ");
                    initStatus = isOgg ?
                        encoder.InitOgg() :
                        encoder.Init();
                    break;
                case Layer.File:
                    Console.Write($"testing {(isOgg ? nameof(encoder.InitOgg) : nameof(encoder.Init))}... ");
                    throw new NotSupportedException();
                //initStatus = isOgg ?
                //    (encoder as libFlaCSharp.Encoder.File).InitOgg(file.Handle) :
                //    (encoder as libFlaCSharp.Encoder.File).Init(file.Handle);
                //break;
                case Layer.FileName:
                    Console.Write($"testing {(isOgg ? nameof(encoder.InitOgg) : nameof(encoder.Init))}... ");
                    initStatus = isOgg ?
                        (encoder as FlaCdotNet.Encoder.File).InitOgg(flacFileName(isOgg)) :
                        (encoder as FlaCdotNet.Encoder.File).Init(flacFileName(isOgg));
                    break;
                default:
                    die_("internal error 001");
                    return false;
            }
            if (initStatus != InitStatus.Ok)
            {
                return die_s_(null, encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetState)}... ");
            var state = encoder.GetState();
            Console.WriteLine($"returned state = {(uint)(State)state} ({state.ToString()})... OK");

            Console.Write($"testing {nameof(encoder.GetVerifyDecoderState)}... ");
            var dstate = encoder.GetVerifyDecoderState();
            Console.WriteLine($"returned state = {(uint)(FlaCdotNet.Decoder.State)dstate} ({dstate.ToString()})... OK");

            {
                ulong absoluteSample;
                uint frameNumber;
                uint channel;
                uint sample;
                int expected;
                int got;

                Console.Write($"testing {nameof(encoder.GetVerifyDecoderErrorStats)}... ");
                encoder.GetVerifyDecoderErrorStats(out absoluteSample, out frameNumber, out channel, out sample, out expected, out got);
                Console.WriteLine("OK");
            }

            Console.Write($"testing {nameof(encoder.GetVerify)}... ");
            if (!encoder.GetVerify())
            {
                Console.Error.WriteLine("FAILED, expected true, got false");
                return false;
            }
            Console.WriteLine("OK");
            Assert.AreEqual(true, encoder.GetVerify(), "expected true, got false");

            Console.Write($"testing {nameof(encoder.GetStreamableSubset)}... ");
            if (!encoder.GetStreamableSubset())
            {
                Console.Error.WriteLine("FAILED, expected true, got false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetDoMidSideStereo)}... ");
            if (encoder.GetDoMidSideStereo())
            {
                Console.Error.WriteLine("FAILED, expected false, got true");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetLooseMidSideStereo)}... ");
            if (encoder.GetLooseMidSideStereo())
            {
                Console.Error.WriteLine("FAILED, expected false, got true");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetChannels)}... ");
            if (encoder.GetChannels() != streamInfo_.Channels)
            {
                Console.Error.WriteLine($"FAILED, expected {streamInfo_.Channels}, got {encoder.GetChannels()}");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetBitsPerSample)}... ");
            if (encoder.GetBitsPerSample() != streamInfo_.BitsPerSample)
            {
                Console.Error.WriteLine($"FAILED, expected {streamInfo_.BitsPerSample}, got {encoder.GetBitsPerSample()}");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetSampleRate)}... ");
            if (encoder.GetSampleRate() != streamInfo_.SampleRate)
            {
                Console.Error.WriteLine($"FAILED, expected {streamInfo_.SampleRate}, got {encoder.GetSampleRate()}");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetBlockSize)}... ");
            if (encoder.GetBlockSize() != streamInfo_.MinBlockSize)
            {
                Console.Error.WriteLine($"FAILED, expected {streamInfo_.MinBlockSize}, got {encoder.GetBlockSize()}");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetMaxLpcOrder)}... ");
            if (encoder.GetMaxLpcOrder() != 0u)
            {
                Console.Error.WriteLine($"FAILED, expected {0}, got {encoder.GetMaxLpcOrder()}");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetQlpCoeffPrecision)}... ");
            encoder.GetQlpCoeffPrecision();
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetDoQlpCoeffPrecSearch)}... ");
            if (encoder.GetDoQlpCoeffPrecSearch())
            {
                Console.Error.WriteLine("FAILED, expected false, got true");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetDoEscapeCoding)}... ");
            if (encoder.GetDoEscapeCoding())
            {
                Console.Error.WriteLine("FAILED, expected false, got true");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetDoExhaustiveModelSearch)}... ");
            if (encoder.GetDoExhaustiveModelSearch())
            {
                Console.Error.WriteLine("FAILED, expected false, got true");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetMinResidualPartitionOrder)}... ");
            if (encoder.GetMinResidualPartitionOrder() != 0u)
            {
                Console.Error.WriteLine($"FAILED, expected {0}, got {encoder.GetMinResidualPartitionOrder()}");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetMaxResidualPartitionOrder)}... ");
            if (encoder.GetMaxResidualPartitionOrder() != 0u)
            {
                Console.Error.WriteLine($"FAILED, expected {0}, got {encoder.GetMaxResidualPartitionOrder()}");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetRiceParameterSearchDist)}... ");
            if (encoder.GetRiceParameterSearchDist() != 0u)
            {
                Console.Error.WriteLine($"FAILED, expected {0}, got {encoder.GetRiceParameterSearchDist()}");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.GetTotalSamplesEstimate)}... ");
            if (encoder.GetTotalSamplesEstimate() != streamInfo_.TotalSamples)
            {
                Console.Error.WriteLine($"FAILED, expected {streamInfo_.TotalSamples}, got {encoder.GetTotalSamplesEstimate()}");
                return false;
            }
            Console.WriteLine("OK");

            for (var i = 0; i < samples.Length; i++)
            {
                samples[i] = i & 7;
                samplesArray[0, i] = samples[i];
            }

            Console.Write($"testing {nameof(encoder.Process)}... ");
            if (!encoder.Process(samplesArray, (uint)samples.LongLength))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.ProcessInterleaved)}... ");
            if (!encoder.ProcessInterleaved(samples, (uint)samples.LongLength))
            {
                return die_s_("returned false", encoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(encoder.Finish)}... ");
            if (!encoder.Finish())
            {
                state = encoder.GetState();
                Console.Error.WriteLine($"FAILED, returned false, state = {(uint)(State)state} ({state.ToString()})");
                return false;
            }
            Console.WriteLine("OK");

            if (layer < Layer.File)
            {
                (encoder as StreamEncoder).file_.Close();
            }

            Console.Write($"freeing {nameof(encoder)} instance... ");
            encoder.Dispose();
            Console.WriteLine("OK");

            Console.WriteLine($"{Environment.NewLine}PASSED!");

            return true;
        }

        [TestMethod]
        public void TestStreamEncoder()
        {
            // Arrange
            bool isOgg = false;

            // Act
            initMetadataBlocks();

            // Assert
            Assert.AreEqual(true, TestStreamEncoder(Layer.Stream, isOgg));
            Assert.AreEqual(true, TestStreamEncoder(Layer.SeekableStream, isOgg));
            //Assert.AreEqual(true, TestStreamEncoder(Layer.File, isOgg));
            Assert.AreEqual(true, TestStreamEncoder(Layer.FileName, isOgg));

            freeMetadataBlocks();
        }

        [TestMethod]
        public void TestOggStreamEncoder()
        {
            // Arrange
            bool isOgg = true;

            // Act
            initMetadataBlocks();

            // Assert
            Assert.AreEqual(true, TestStreamEncoder(Layer.Stream, isOgg));
            Assert.AreEqual(true, TestStreamEncoder(Layer.SeekableStream, isOgg));
            //Assert.AreEqual(true, TestStreamEncoder(Layer.File, isOgg));
            Assert.AreEqual(true, TestStreamEncoder(Layer.FileName, isOgg));

            freeMetadataBlocks();
        }
    }
}
