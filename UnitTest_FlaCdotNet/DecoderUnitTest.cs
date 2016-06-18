using System;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using FlaCdotNet;
using FlaCdotNet.Decoder;
using FlaCdotNet.Metadata;

namespace UnitTest_FlaCdotNet
{
    [TestClass]
    public class DecoderUnitTest
    {
        public enum Layer
        {
            Stream = 0,
            SeekableStream,
            File,
            FileName
        }

        static string[] LayerString =
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
        static Prototype[] expectedMetadataSequence_ = new Prototype[9];
        static int numExpected_;
        static long flacFileSize_;

        struct StreamDecoderClientData
        {
            public Layer Layer;
            public FileStream File;
            public string FileName;
            public uint CurrentMetadataNumber;
            public bool IgnoreErrors;
            public bool ErrorOccured;
        }

        static string flacFileName(bool isOgg)
        {
            return isOgg ? "metadata.oga" : "metadata.flac";
        }

        static bool die_(string msg)
        {
            Console.Error.WriteLine($"ERROR: {msg}");
            return false;
        }

        static bool die_s_(string msg, [Pure]FlaCdotNet.Decoder.Stream decoder)
        {
            var state = decoder.GetState();

            if (!string.IsNullOrWhiteSpace(msg))
            {
                Console.Error.Write($"FAILED, {msg}");
            }
            else
            {
                Console.Error.Write("FAILED");
            }
            Console.Error.WriteLine($", state = {(int)(State)state} ({state.ToString()})");

            return false;
        }

        static void openTestFile(ref StreamDecoderClientData pdcd, bool isOgg, FileMode mode)
        {
            pdcd.FileName = flacFileName(isOgg);
            pdcd.File = System.IO.File.Open(pdcd.FileName, mode);
        }

        static void initMetadataBlocks()
        {
            TestCommon.multis_init_metadata_blocks(streamInfo_, padding_, seekTable_, application1_, application2_, vorbisComment_, cueSheet_, picture_, unknown_);
        }

        static void freeMetadataBlocks()
        {
            TestCommon.mutils__free_metadata_blocks(streamInfo_, padding_, seekTable_, application1_, application2_, vorbisComment_, cueSheet_, picture_, unknown_);
        }

        static bool generateFile(bool isOgg)
        {
            Console.WriteLine($"{Environment.NewLine}{Environment.NewLine}generating {(isOgg ? "Ogg " : string.Empty)}FLAC file for decoder tests...");

            numExpected_ = 0;
            expectedMetadataSequence_[numExpected_++] = padding_;
            expectedMetadataSequence_[numExpected_++] = seekTable_;
            expectedMetadataSequence_[numExpected_++] = application1_;
            expectedMetadataSequence_[numExpected_++] = application2_;
            expectedMetadataSequence_[numExpected_++] = vorbisComment_;
            expectedMetadataSequence_[numExpected_++] = cueSheet_;
            expectedMetadataSequence_[numExpected_++] = picture_;
            expectedMetadataSequence_[numExpected_++] = unknown_;
            var seq = expectedMetadataSequence_.Take(numExpected_).ToArray();

            if (!TestCommon.file_utils__generate_flacfile(isOgg, flacFileName(isOgg), ref flacFileSize_, 512 * 1024, streamInfo_, seq))
            {
                return die_("creating the encoded file");
            }

            return true;
        }

        public interface DecoderCommon
        {
            WriteStatus CommonWriteCallback([Pure]Frame frame);

            void CommonMetadataCallback([Pure]Prototype metadata);

            void CommonErrorCallback(ErrorStatus status);

            bool TestRespond(bool isOgg);
        }

        public class StreamDecoder
            : FlaCdotNet.Decoder.Stream,
            DecoderCommon
        {
            public Layer layer_;
            public uint currentMetadataNumber_;
            public bool ignoreError_;
            public bool errorOccured_;
            public FileStream file_;

            public StreamDecoder(Layer layer)
                : base()
            {
                layer_ = layer;
                currentMetadataNumber_ = 0;
                ignoreError_ = false;
                errorOccured_ = false;
            }

            public WriteStatus CommonWriteCallback([Pure]Frame frame)
            {
                if (errorOccured_)
                {
                    return WriteStatus.Abort;
                }
                
                if (
                    (frame.Header.NumberType == FrameNumberType.FrameNumber  && frame.Header.FrameNumber == 0) ||
                    (frame.Header.NumberType == FrameNumberType.SampleNumber && frame.Header.SampleNumber == 0)
                )
                {
                    Console.Write("content... ");
                    Console.Out.Flush();
                }

                return WriteStatus.Continue;
            }

            public void CommonMetadataCallback([Pure]Prototype metadata)
            {
                if (errorOccured_)
                {
                    return;
                }

                Console.Write($"{currentMetadataNumber_}... ");
                Console.Out.Flush();

                if (currentMetadataNumber_ >= numExpected_)
                {
                    die_("got more metadata blocks than expected");
                    errorOccured_ = true;
                }
                else
                {
                    if (!expectedMetadataSequence_[currentMetadataNumber_].Equals(metadata))
                    {
                        die_("metadata block mismatch");
                        errorOccured_ = true;
                    }
                }
                currentMetadataNumber_++;
            }

            public void CommonErrorCallback(ErrorStatus status)
            {
                if (!ignoreError_)
                {
                    Console.WriteLine($"ERROR: got error callback: err = {(uint)status} ({status.ToString()})");
                    errorOccured_ = true;
                }
            }

            protected override ReadStatus readCallback(byte[] buffer, ref uint bytes)
            {
                uint requestedBytes = bytes;

                if (errorOccured_)
                {
                    return ReadStatus.Abort;
                }

                if (file_.Position == file_.Length)
                {
                    bytes = 0u;
                    return ReadStatus.EndOfStream;
                }
                else if (requestedBytes > 0u)
                {
                    bytes = (uint)file_.Read(buffer, 0, (int)requestedBytes);
                    if (bytes == 0)
                    {
                        if (file_.Position == file_.Length)
                        {
                            return ReadStatus.EndOfStream;
                        }
                        else
                        {
                            return ReadStatus.Continue;
                        }
                    }
                    else
                    {
                        return ReadStatus.Continue;
                    }
                }
                else
                {
                    return ReadStatus.Abort;
                }
            }

            protected override SeekStatus seekCallback(ulong absoluteByteOffset)
            {
                if (layer_ == Layer.Stream)
                {
                    return SeekStatus.Unsupported;
                }

                if (errorOccured_)
                {
                    return SeekStatus.Error;
                }

                if (file_.Seek((long)absoluteByteOffset, SeekOrigin.Begin) < 0)
                {
                    errorOccured_ = true;
                    return SeekStatus.Error;
                }

                return SeekStatus.Ok;
            }

            protected override TellStatus tellCallback(ref ulong absoluteByteOffset)
            {
                if (layer_ == Layer.Stream)
                {
                    return TellStatus.Unsupported;
                }

                if (errorOccured_)
                {
                    return TellStatus.Error;
                }

                var offset = file_.Position;

                if (offset < 0)
                {
                    errorOccured_ = true;
                    return TellStatus.Error;
                }

                absoluteByteOffset = (ulong)offset;
                return TellStatus.Ok;
            }

            protected override LengthStatus lengthCallback(ref ulong streamLength)
            {
                if (layer_ == Layer.Stream)
                {
                    return LengthStatus.Unsupported;
                }

                if (errorOccured_)
                {
                    return LengthStatus.Error;
                }

                streamLength = (ulong)flacFileSize_;
                return LengthStatus.Ok;
            }

            protected override bool eofCallback()
            {
                if (layer_ == Layer.Stream)
                {
                    return false;
                }

                if (errorOccured_)
                {
                    return true;
                }

                return file_.Position == file_.Length;
            }

            protected override WriteStatus writeCallback([Pure] Frame frame, [Pure] int[,] buffer)
            {
                return CommonWriteCallback(frame);
            }

            protected override void metadataCallback([Pure] Prototype metadata)
            {
                CommonMetadataCallback(metadata);
            }

            protected override void errorCallback(ErrorStatus status)
            {
                CommonErrorCallback(status);
            }

            public bool TestRespond(bool isOgg)
            {
                InitStatus initStatus;

                if (!SetMD5Checking(true))
                {
                    Console.Error.WriteLine($"FAILED at {nameof(SetMD5Checking)}(), returned false");
                    return false;
                }

                Console.Write($"testing {(isOgg ? nameof(InitOgg) : nameof(Init))}()... ");
                initStatus = isOgg ? InitOgg() : Init();
                if (initStatus != InitStatus.Ok)
                {
                    return die_s_(null, this);
                }
                Console.WriteLine("OK");

                currentMetadataNumber_ = 0;

                if (file_.Seek(0, SeekOrigin.Begin) < 0)
                {
                    Console.Error.WriteLine($"FAILED rewinding input, NO errno set for C#");
                    return false;
                }

                Console.Write($"testing {nameof(ProcessUntilEndOfStream)}... ");
                if (!ProcessUntilEndOfStream())
                {
                    State state = GetState();
                    Console.Error.WriteLine($"FAILED, returned false, state = {(uint)(FlaCdotNet.Decoder.State)state} ({state.ToString()})");
                    return false;
                }
                Console.WriteLine("OK");

                Console.Write($"testing {nameof(Finish)}()... ");
                if (!Finish())
                {
                    State state = GetState();
                    Console.Error.WriteLine($"FAILED, returned false, state = {(uint)(FlaCdotNet.Decoder.State)state} ({state.ToString()})");
                    return false;
                }
                Console.WriteLine("OK");

                return true;
            }
        }

        public class FileDecoder
            : FlaCdotNet.Decoder.File,
            DecoderCommon
        {
            public Layer layer_;
            public uint currentMetadataNumber_;
            public bool ignoreError_;
            public bool errorOccured_;

            public FileDecoder(Layer layer)
                : base()
            {
                layer_ = layer;
            }

            public WriteStatus CommonWriteCallback([Pure]Frame frame)
            {
                if (errorOccured_)
                {
                    return WriteStatus.Abort;
                }
                
                if (
                    (frame.Header.NumberType == FrameNumberType.FrameNumber  && frame.Header.FrameNumber == 0) ||
                    (frame.Header.NumberType == FrameNumberType.SampleNumber && frame.Header.SampleNumber == 0)
                )
                {
                    Console.Write("content... ");
                    Console.Out.Flush();
                }

                return WriteStatus.Continue;
            }

            public void CommonMetadataCallback([Pure]Prototype metadata)
            {
                if (errorOccured_)
                {
                    return;
                }

                Console.Write($"{currentMetadataNumber_}... ");
                Console.Out.Flush();

                if (currentMetadataNumber_ >= numExpected_)
                {
                    die_("got more metadata blocks than expected");
                    errorOccured_ = true;
                }
                else
                {
                    if (!expectedMetadataSequence_[currentMetadataNumber_].Equals(metadata))
                    {
                        die_("metadata block mismatch");
                        errorOccured_ = true;
                    }
                }
                currentMetadataNumber_++;
            }

            public void CommonErrorCallback(ErrorStatus status)
            {
                if (!ignoreError_)
                {
                    Console.WriteLine($"ERROR: got error callback: err = {(uint)status} ({status.ToString()})");
                    errorOccured_ = true;
                }
            }

            protected override WriteStatus writeCallback([Pure] Frame frame, [Pure] int[,] buffer)
            {
                return CommonWriteCallback(frame);
            }

            protected override void metadataCallback([Pure] Prototype metadata)
            {
                CommonMetadataCallback(metadata);
            }

            protected override void errorCallback(ErrorStatus status)
            {
                CommonErrorCallback(status);
            }

            public bool TestRespond(bool isOgg)
            {
                InitStatus initStatus;

                if (!SetMD5Checking(true))
                {
                    Console.Error.WriteLine($"FAILED at {nameof(SetMD5Checking)}(), returned false");
                    return false;
                }

                switch (layer_)
                {
                    case Layer.File:
                        throw new PlatformNotSupportedException();
                    case Layer.FileName:
                        Console.Write($"testing {(isOgg ? nameof(InitOgg) : nameof(Init))}()... ");
                        initStatus = isOgg ? InitOgg(flacFileName(isOgg)) : Init(flacFileName(isOgg));
                        break;
                    default:
                        die_("internal error 001");
                        return false;
                }
                if (initStatus != InitStatus.Ok)
                {
                    return die_s_(null, this);
                }
                Console.WriteLine("OK");

                currentMetadataNumber_ = 0;

                Console.Write($"testing {nameof(ProcessUntilEndOfStream)}()... ");
                if (!ProcessUntilEndOfStream())
                {
                    State state = GetState();
                    Console.Error.WriteLine($"FAILED, returned false, state = {(uint)(FlaCdotNet.Decoder.State)state} ({state.ToString()})");
                    return false;
                }
                Console.WriteLine("OK");

                Console.Write($"testing {nameof(Finish)}()... ");
                if (!Finish())
                {
                    State state = GetState();
                    Console.Error.WriteLine($"FAILED, returned false, state = {(uint)(FlaCdotNet.Decoder.State)state} ({state.ToString()})");
                    return false;
                }
                Console.WriteLine("OK");

                return true;
            }
        }

        static FlaCdotNet.Decoder.Stream NewByLayer(Layer layer)
        {
            if (layer < Layer.File)
            {
                return new StreamDecoder(layer);
            }
            else
            {
                return new FileDecoder(layer);
            }
        }

        static bool TestStreamDecoder(Layer layer, bool isOgg)
        {
            FlaCdotNet.Decoder.Stream decoder;
            FlaCdotNet.Decoder.InitStatus initStatus;
            bool expect;

            Console.WriteLine($"{Environment.NewLine}+++ {nameof(FlaCdotNet)} unit test: {(layer < Layer.File ? nameof(FlaCdotNet.Decoder.Stream) : nameof(FlaCdotNet.Decoder.File))} (layer: {LayerString[(int)layer]}, format: {(isOgg ? "Ogg FLAC" : "FLAC")}){Environment.NewLine}");

            //
            // test new -> Dispose
            //
            Console.Write($"allocating {nameof(decoder)} instance... ");
            decoder = NewByLayer(layer);
            if (decoder == null)
            {
                Console.Error.WriteLine("FAILED, new returned NULL");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.IsValid)}()... ");
            if (!decoder.IsValid())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"freeing {nameof(decoder)} instance... ");
            decoder.Dispose();
            Console.WriteLine("OK");

            //
            // test new -> init -> Dispose
            //
            Console.Write("allocating decoder instance... ");
            decoder = NewByLayer(layer);
            if (decoder == null)
            {
                Console.Error.WriteLine("FAILED, new returned NULL");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.IsValid)}()... ");
            if (!decoder.IsValid())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {(isOgg ? nameof(decoder.InitOgg) : nameof(decoder.Init))}()... ");
            switch (layer)
            {
                case Layer.Stream:
                    goto case Layer.SeekableStream;
                case Layer.SeekableStream:
                    (decoder as StreamDecoder).file_ = Console.OpenStandardInput() as FileStream;
                    initStatus = isOgg ? decoder.InitOgg() : decoder.Init();
                    break;
                case Layer.File:
                    throw new PlatformNotSupportedException();
                case Layer.FileName:
                    initStatus = isOgg ?
                        (decoder as FileDecoder).InitOgg(flacFileName(isOgg)) :
                        (decoder as FileDecoder).Init(flacFileName(isOgg));
                    break;
                default:
                    die_("internal error 006");
                    return false;
            }
            if (initStatus != InitStatus.Ok)
            {
                return die_s_(null, decoder);
            }
            Console.WriteLine("OK");

            Console.Write($"freeing {nameof(decoder)} instance... ");
            decoder.Dispose();
            Console.WriteLine("OK");

            //
            // test normal usage
            //
            numExpected_ = 0;
            expectedMetadataSequence_[numExpected_++] = streamInfo_;

            Console.Write("allocating decoder instance... ");
            decoder = NewByLayer(layer);
            if (decoder == null)
            {
                Console.Error.WriteLine("FAILED, new returned NULL");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.IsValid)}()... ");
            if (!decoder.IsValid())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            if (isOgg)
            {
                Console.Write($"testing {nameof(decoder.SetOggSerialNumber)}()... ");
                if (!decoder.SetOggSerialNumber(TestCommon.file_utils__ogg_serial_number))
                {
                    return die_s_("returned false", decoder);
                }
                Console.WriteLine("OK");
            }

            if (!decoder.SetMD5Checking(true))
            {
                Console.Error.WriteLine($"FAILED at {nameof(decoder.SetMD5Checking)}(), returned false");
            }

            switch (layer)
            {
                case Layer.Stream:
                    goto case Layer.SeekableStream;
                case Layer.SeekableStream:
                    try
                    {
                        (decoder as StreamDecoder).file_ = System.IO.File.Open(flacFileName(isOgg), FileMode.Open, FileAccess.Read);
                    }
                    catch (Exception e) when (!(e is ArgumentException) && !(e is ArgumentNullException) && !(e is ArgumentOutOfRangeException))
                    {
                        Console.Error.WriteLine($"ERROR {e.Message}");
                        return false;
                    }
                    Console.WriteLine("OK");

                    Console.Write($"testing {(isOgg ? nameof(decoder.InitOgg) : nameof(decoder.Init))}()... ");
                    initStatus = isOgg ?
                        decoder.InitOgg() :
                        decoder.Init();
                    break;
                case Layer.File:
                    throw new PlatformNotSupportedException();
                case Layer.FileName:
                    Console.Write($"testing {(isOgg ? nameof(decoder.InitOgg) : nameof(decoder.Init))}()... ");
                    initStatus = isOgg ?
                        (decoder as FlaCdotNet.Decoder.File).InitOgg(flacFileName(isOgg)) :
                        (decoder as FlaCdotNet.Decoder.File).Init(flacFileName(isOgg));
                    break;
                default:
                    die_("internal error 009");
                    return false;
            }
            if ((FlaCdotNet.Decoder.InitStatus)initStatus != FlaCdotNet.Decoder.InitStatus.Ok)
            {
                return die_s_(null, decoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.GetState)}()... ");
            FlaCdotNet.Decoder.Stream.State state = decoder.GetState();
            Console.WriteLine($"returned state = {(uint)(State)state} ({state.ToString()})... OK");

            (decoder as StreamDecoder).currentMetadataNumber_ = 0;
            (decoder as StreamDecoder).ignoreError_ = false;
            (decoder as StreamDecoder).errorOccured_ = false;

            Console.Write($"testing {nameof(decoder.GetMD5Checking)}()... ");
            if (!decoder.GetMD5Checking())
            {
                Console.Error.WriteLine("FAILED, returned false, expect true");
            }
            Console.WriteLine("OK");
            Assert.AreEqual(true, decoder.GetMD5Checking(), "returned false, expect true");

            Console.Write($"testing {nameof(decoder.ProcessUntilEndOfMetadata)}()... ");
            if (!decoder.ProcessUntilEndOfMetadata())
            {
                return die_s_("returned false", decoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.ProcessSingle)}()... ");
            if (!decoder.ProcessSingle())
            {
                return die_s_("returned false", decoder);
            }
            Console.WriteLine("OK");

            Console.WriteLine($"testing {nameof(decoder.SkipSingleFrame)}()... ");
            if (!decoder.SkipSingleFrame())
            {
                return die_s_("returned false", decoder);
            }
            Console.WriteLine("OK");

            if (layer < Layer.File)
            {
                Console.Write($"testing {nameof(decoder.Flush)}()... ");
                if (!decoder.Flush())
                {
                    return die_s_("returned false", decoder);
                }
                Console.WriteLine("OK");

                (decoder as StreamDecoder).ignoreError_ = true;
                Console.Write($"testing {nameof(decoder.ProcessSingle)}()... ");
                if (!decoder.ProcessSingle())
                {
                    return die_s_("returned false", decoder);
                }
                Console.WriteLine("OK");
                (decoder as StreamDecoder).ignoreError_ = false;
            }

            expect = (layer != Layer.Stream);
            Console.Write($"testing {nameof(decoder.SeekAbsolute)}()... ");
            if (decoder.SeekAbsolute(0u) != expect)
            {
                return die_s_(expect ? "returned false" : "returned true", decoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.ProcessUntilEndOfStream)}()... ");
            if (!decoder.ProcessUntilEndOfStream())
            {
                return die_s_("returned false", decoder);
            }
            Console.WriteLine("OK");

            expect = (layer != Layer.Stream);
            Console.Write($"testing {nameof(decoder.SeekAbsolute)}()... ");
            if (decoder.SeekAbsolute(0u) != expect)
            {
                return die_s_(expect ? "returned false" : "returned true", decoder);
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.GetChannels)}()... ");
            {
                uint channels = decoder.GetChannels();
                if (channels != streamInfo_.Channels)
                {
                    Console.Error.WriteLine($"FAILED, returned {channels}, expected {streamInfo_.Channels}");
                    return false;
                }
                Assert.AreEqual(streamInfo_.Channels, channels, "FAILED");
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.GetBitsPerSample)}()... ");
            {
                uint bitsPerSample = decoder.GetBitsPerSample();
                if (bitsPerSample != streamInfo_.BitsPerSample)
                {
                    Console.Error.WriteLine($"FAILED, returned {bitsPerSample}, expected {streamInfo_.BitsPerSample}");
                    return false;
                }
                Assert.AreEqual(streamInfo_.BitsPerSample, bitsPerSample, "FAILED");
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.GetSampleRate)}()... ");
            {
                uint sampleRate = decoder.GetSampleRate();
                if (sampleRate != streamInfo_.SampleRate)
                {
                    Console.Error.WriteLine($"FAILED, returned {sampleRate}, expected {streamInfo_.SampleRate}");
                    return false;
                }
                Assert.AreEqual(streamInfo_.SampleRate, sampleRate, "FAILED");
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.GetBlockSize)}()... ");
            {
                uint blockSize = decoder.GetBlockSize();
                Console.WriteLine($"returned {blockSize}... {(blockSize > 0u ? "OK" : "FAILED")}");
                if (blockSize == 0u)
                {
                    return false;
                }
            }

            Console.Write($"testing {nameof(decoder.GetChannelAssignment)}()... ");
            {
                ChannelAssignment ca = decoder.GetChannelAssignment();
                Console.WriteLine($"returned {(uint)ca}... {ca.ToString()}");
            }

            if (layer < Layer.File)
            {
                Console.Write($"testing {nameof(decoder.Reset)}()... ");
                if (!decoder.Reset())
                {
                    return die_s_("returned false", decoder);
                }
                Console.WriteLine("OK");

                if (layer == Layer.Stream)
                {
                    Console.Write("rewinding input... ");
                    if ((decoder as StreamDecoder).file_.Seek(0L, SeekOrigin.Begin) < 0L)
                    {
                        Console.Error.WriteLine("FAILED, NO ERRNO for C#");
                        return false;
                    }
                    Console.WriteLine("OK");
                }

                (decoder as StreamDecoder).currentMetadataNumber_ = 0;

                Console.Write($"testing {nameof(decoder.ProcessUntilEndOfStream)}()... ");
                if (!decoder.ProcessUntilEndOfStream())
                {
                    return die_s_("returned false", decoder);
                }
                Console.WriteLine("OK");
            }

            Console.Write($"testing {nameof(decoder.Finish)}()... ");
            if (!decoder.Finish())
            {
                state = decoder.GetState();
                Console.Error.WriteLine($"FAILED, returned false, state = {(uint)(State)state} ({state.ToString()})");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write("testing {nameof(decoder.SetMetadataRespondAll)}()... ");
            if (!decoder.SetMetadataRespondAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            if (isOgg)
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = application1_;
                expectedMetadataSequence_[numExpected_++] = application2_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                expectedMetadataSequence_[numExpected_++] = unknown_;
            }
            else
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = application1_;
                expectedMetadataSequence_[numExpected_++] = application2_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                expectedMetadataSequence_[numExpected_++] = unknown_;
            }

            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreAll)}()... ");
            if (!decoder.SetMetadataIgnoreAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataRespondAll)}()... ");
            if (!decoder.SetMetadataRespondAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataIgnore)}({nameof(MetadataType.VorbisComment)})... ");
            if (!decoder.SetMetadataIgnore(MetadataType.VorbisComment))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            expectedMetadataSequence_[numExpected_++] = streamInfo_;
            expectedMetadataSequence_[numExpected_++] = padding_;
            expectedMetadataSequence_[numExpected_++] = seekTable_;
            expectedMetadataSequence_[numExpected_++] = application1_;
            expectedMetadataSequence_[numExpected_++] = application2_;
            expectedMetadataSequence_[numExpected_++] = cueSheet_;
            expectedMetadataSequence_[numExpected_++] = picture_;
            expectedMetadataSequence_[numExpected_++] = unknown_;

            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataRespondAll)}()... ");
            if (!decoder.SetMetadataRespondAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataIgnore)}({nameof(MetadataType.Application)})... ");
            if (!decoder.SetMetadataIgnore(MetadataType.Application))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            if (isOgg)
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                //expectedMetadataSequence_[numExpected_++] = unknown_;
            }
            else
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                //expectedMetadataSequence_[numExpected_++] = unknown_;
            }

            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataRespondAll)}()... ");
            if (!decoder.SetMetadataRespondAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreApplication)}(of app block #1)... ");
            if (!decoder.SetMetadataIgnoreApplication(application1_.ID))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            if (isOgg)
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = application2_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                expectedMetadataSequence_[numExpected_++] = unknown_;
            }
            else
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = application2_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                expectedMetadataSequence_[numExpected_++] = unknown_;
            }

            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataRespondAll)}()... ");
            if (!decoder.SetMetadataRespondAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreApplication)}(of app block #1)... ");
            if (!decoder.SetMetadataIgnoreApplication(application1_.ID))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreApplication)}(of app block #2)... ");
            if (!decoder.SetMetadataIgnoreApplication(application2_.ID))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            if (isOgg)
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                expectedMetadataSequence_[numExpected_++] = unknown_;
            }
            else
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                expectedMetadataSequence_[numExpected_++] = unknown_;
            }

            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreAll)}()... ");
            if (!decoder.SetMetadataIgnoreAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataRespond)}({nameof(MetadataType.VorbisComment)})... ");
            if (!decoder.SetMetadataRespond(MetadataType.VorbisComment))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            expectedMetadataSequence_[numExpected_++] = vorbisComment_;
            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreAll)}()... ");
            if (!decoder.SetMetadataIgnoreAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataRespond)}({nameof(MetadataType.Application)})... ");
            if (!decoder.SetMetadataRespond(MetadataType.Application))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            expectedMetadataSequence_[numExpected_++] = application1_;
            expectedMetadataSequence_[numExpected_++] = application2_;
            // !!!!!!!!!!
            expectedMetadataSequence_[numExpected_++] = unknown_;
            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreAll)}()... ");
            if (!decoder.SetMetadataIgnoreAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataRespondApplication)}(of app block #1)... ");
            if (!decoder.SetMetadataRespondApplication(application1_.ID))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            expectedMetadataSequence_[numExpected_++] = application1_;
            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreAll)}()... ");
            if (!decoder.SetMetadataIgnoreAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataRespondApplication)}(of app block #1)... ");
            if (!decoder.SetMetadataRespondApplication(application1_.ID))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataRespondApplication)}(of app block #2)... ");
            if (!decoder.SetMetadataRespondApplication(application2_.ID))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            expectedMetadataSequence_[numExpected_++] = application1_;
            expectedMetadataSequence_[numExpected_++] = application2_;
            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataRespondAll)}()... ");
            if (!decoder.SetMetadataIgnoreAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataIgnore)}({nameof(MetadataType.Application)})... ");
            if (!decoder.SetMetadataIgnore(MetadataType.Application))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataRespondApplication)}(of app block #1)... ");
            if (!decoder.SetMetadataRespondApplication(application1_.ID))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            if (isOgg)
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = application1_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                //expectedMetadataSequence_[numExpected_++] = unknown_;
            }
            else
            {
                expectedMetadataSequence_[numExpected_++] = streamInfo_;
                expectedMetadataSequence_[numExpected_++] = padding_;
                expectedMetadataSequence_[numExpected_++] = seekTable_;
                expectedMetadataSequence_[numExpected_++] = application1_;
                expectedMetadataSequence_[numExpected_++] = vorbisComment_;
                expectedMetadataSequence_[numExpected_++] = cueSheet_;
                expectedMetadataSequence_[numExpected_++] = picture_;
                //expectedMetadataSequence_[numExpected_++] = unknown_;
            }
            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreAll)}()... ");
            if (!decoder.SetMetadataIgnoreAll())
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataRespond)}({nameof(MetadataType.Application)})... ");
            if (!decoder.SetMetadataRespond(MetadataType.Application))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            Console.Write($"testing {nameof(decoder.SetMetadataIgnoreApplication)}(of app block #1)... ");
            if (!decoder.SetMetadataIgnoreApplication(application1_.ID))
            {
                Console.Error.WriteLine("FAILED, returned false");
                return false;
            }
            Console.WriteLine("OK");

            numExpected_ = 0;
            expectedMetadataSequence_[numExpected_++] = application2_;
            // !!!!!!!!!!
            expectedMetadataSequence_[numExpected_++] = unknown_;
            if (!(layer < Layer.File ? (decoder as StreamDecoder)?.TestRespond(isOgg) : (decoder as FileDecoder)?.TestRespond(isOgg)).Value)
            {
                return false;
            }

            if (layer < Layer.File)
            {
                (decoder as StreamDecoder)?.file_.Close();
            }

            Console.Write($"freeing {nameof(decoder)} instance... ");
            decoder.Dispose();
            Console.WriteLine("OK");

            Console.WriteLine($"{Environment.NewLine}PASSED!");

            return true;
        }

        [TestMethod]
        public void TestStreamDecoder()
        {
            // Arrange
            var isOgg = false;

            // Act
            initMetadataBlocks();

            // Assert
            Assert.AreEqual(true, generateFile(isOgg));
            Assert.AreEqual(true, TestStreamDecoder(Layer.Stream, isOgg));
            Assert.AreEqual(true, TestStreamDecoder(Layer.SeekableStream, isOgg));
            //Assert.AreEqual(true, TestStreamDecoder(Layer.File, isOgg));
            Assert.AreEqual(true, TestStreamDecoder(Layer.FileName, isOgg));

            freeMetadataBlocks();
        }

        [TestMethod]
        public void TestOggStreamDecoder()
        {
            // Arrange
            var isOgg = true;

            // Act
            initMetadataBlocks();

            // Assert
            Assert.AreEqual(true, generateFile(isOgg));
            Assert.AreEqual(true, TestStreamDecoder(Layer.Stream, isOgg));
            Assert.AreEqual(true, TestStreamDecoder(Layer.SeekableStream, isOgg));
            //Assert.AreEqual(true, TestStreamDecoder(Layer.File, isOgg));
            Assert.AreEqual(true, TestStreamDecoder(Layer.FileName, isOgg));

            freeMetadataBlocks();
        }
    }
}
