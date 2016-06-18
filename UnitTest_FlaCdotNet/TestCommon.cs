using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using FlaCdotNet;
using FlaCdotNet.Metadata;

namespace UnitTest_FlaCdotNet
{
    internal static class Constants
    {
        public static string VendorString = $"reference libFLAC 1.3.1 20141125";
    }

    internal static class TestCommon
    {
        public const int file_utils__ogg_serial_number = 12345;

        public static void multis_init_metadata_blocks(StreamInfo streamInfo, Padding padding, SeekTable seekTable, Application application1, Application application2, VorbisComment vorbisComment, CueSheet cueSheet, Picture picture, /*Unknown*/Application unknown)
        {
            streamInfo.IsLast = false;
            streamInfo.MinBlockSize = 576u;
            streamInfo.MaxBlockSize = 576u;
            streamInfo.MinFrameSize = 0u;
            streamInfo.MaxFrameSize = 0u;
            streamInfo.SampleRate = 44100u;
            streamInfo.Channels = 1u;
            streamInfo.BitsPerSample = 8u;
            streamInfo.TotalSamples = 0u;
            streamInfo.MD5Sum = new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            //streamInfo.Length = 34u;

            padding.IsLast = false;
            padding.Length = 1234u;

            seekTable.IsLast = false;
            //seekTable.NumPoints = 2;
            //seekTable.TemplateAppendPlaceholders(2u);
            //seekTable[0].SampleNumber = 0;
            //seekTable[0].StreamOffset = 0;
            //seekTable[0].FrameSamples = streamInfo.MinBlockSize;
            {
                var point = new SeekTable.SeekPoint();
                point.SampleNumber = 0;
                point.StreamOffset = 0;
                point.FrameSamples = streamInfo.MinBlockSize;
                seekTable.InsertPoint(0u, point);
            }
            //seekTable[1].SampleNumber = ulong.MaxValue;
            //seekTable[1].StreamOffset = 1000;
            //seekTable[1].FrameSamples = streamInfo.MinBlockSize;
            {
                var point = new SeekTable.SeekPoint();
                point.SampleNumber = ulong.MaxValue;
                point.StreamOffset = 1000;
                point.FrameSamples = streamInfo.MinBlockSize;
                seekTable.InsertPoint(1u, point);
            }
            //seekTable.Length = seekTable.NumPoints * 18u;

            application1.IsLast = false;
            application1.ID = Encoding.ASCII.GetBytes("This");
            application1.Data = new byte[] { 0xF0, 0xE1, 0xD2, 0xC3 };
            //application1.Length = 8;

            application2.IsLast = false;
            application2.ID = Encoding.ASCII.GetBytes("Here");
            application2.SetData(null, false);
            //application2.Length = 4;

            {
                var vendorString = Constants.VendorString;
                //var vendorString = vorbisComment.VendorString;
                vorbisComment.IsLast = false;
                vorbisComment.VendorString = vendorString;
                //vorbisComment.ResizeComments(2u);
                //vorbisComment[0u].Field = "ab=cd";
                //vorbisComment[1u].Field = string.Empty;
                vorbisComment.AppendComment(new VorbisComment.Entry("ab=cd"));
                //vorbisComment.AppendComment(new VorbisComment.Entry("C=#"));
                vorbisComment.ResizeComments(2u);
                //vorbisComment.Length = (4u + (uint)vendorString.LongCount()) + 4u + (4u + 5u) + (4u + 0u);
            }

            cueSheet.IsLast = false;
            cueSheet.MediaCatalogNumber = "jC";
            cueSheet.LeadIn = 2ul * 44100ul;
            cueSheet.IsCD = true;
            //cueSheet.ResizeTracks(3u);

            //cueSheet.InsertBlankTrack(0u);
            //cueSheet[0u].Offset = 0ul;
            //cueSheet[0u].Number = /*(byte)*/1;
            //cueSheet[0u].ISRC = "ABCDE1234567";
            //cueSheet[0u].TrackType = 0u;
            //cueSheet[0u].PreEmphasis = true;
            {
                var track = new CueSheet.Track();
                track.Offset = 0ul;
                track.Number = /*(byte)*/1;
                track.ISRC = "ACBDE1234567";
                track.TrackType = 0u;
                track.PreEmphasis = true;
                cueSheet.InsertTrack(0u, track);
            }
            //cueSheet.ResizeIndices(0u, 2u);

            //cueSheet.InsertBlankIndex(0u, 0u);
            //cueSheet[0u][0u].Offset = 0ul;
            //cueSheet[0u][0u].Number = /*(byte)*/0/*b*/;
            {
                var index = new CueSheet.Index();
                index.Offset = 0ul;
                index.Number = /*(byte)*/0;
                cueSheet.InsertIndex(0u, 0u, index);
            }
            //cueSheet.InsertBlankIndex(0u, 1u);
            //cueSheet[0u][1u].Offset = 123ul * 588ul;
            //cueSheet[0u][1u].Number = /*(byte)*/1/*b*/;
            {
                var index = new CueSheet.Index();
                index.Offset = 123ul * 588ul;
                index.Number = /*(byte)*/1/*b*/;
                cueSheet.InsertIndex(0u, 1u, index);
            }
            //cueSheet.InsertBlankTrack(1u);
            //cueSheet[1u].Offset = 1234ul * 588ul;
            //cueSheet[1u].Number = /*(byte)*/2/*b*/;
            //cueSheet[1u].ISRC = "ACBDE7654321";
            //cueSheet[1u].TrackType = 1u;
            //cueSheet[1u].PreEmphasis = false;
            {
                var track = new CueSheet.Track();
                track.Offset = 1234ul * 588ul;
                track.Number = /*(byte)*/2/*b*/;
                track.ISRC = "ACBDE7654321";
                track.TrackType = 1u;
                track.PreEmphasis = false;
                cueSheet.InsertTrack(1u, track);
            }
            //cueSheet.ResizeIndices(1u, 1u);

            //cueSheet[1u][0u].Offset = 0ul;
            //cueSheet[1u][0u].Number = /*(byte)*/1/*b*/;
            {
                var index = new CueSheet.Index();
                index.Offset = 0ul;
                index.Number = /*(byte)*/1/*b*/;
                cueSheet.InsertIndex(1u, 0u, index);
            }
            //cueSheet.InsertBlankTrack(2u);
            //cueSheet[2u].Offset = 12345ul * 588ul;
            //cueSheet[2u].Number = /*(byte)170*/0xAA;
            {
                var track = new CueSheet.Track();
                track.Offset = 12345ul * 588ul;
                track.Number = /*(byte)170*/0xAA;
                cueSheet.InsertTrack(2u, track);
            }
            //cueSheet.ResizeIndices(2u, 0u);
#if false
            cueSheet.Length =
                /* cuesheet guts */
                (
                    128 * 8 +
                    64 +
                    1 +
                    7 + 258 * 8 +
                    8
                ) / 8 +
                /* 2 tracks */
                3 * (
                    64 +
                    8 +
                    12 * 8 +
                    1 +
                    1 +
                    6 + 13 * 8 +
                    8
                ) / 8 +
                /* 3 index points */
                3 * (
                    64 +
                    8 +
                    3 * 8
                ) / 8
            ;
#endif

            picture.IsLast = false;
#if false
            picture.Length =
                (
                    32 +
                    32 +
                    32 +
                    32 +
                    32 +
                    32 +
                    32 +
                    32
                ) / 8
            ;
#endif
            picture.PictureType = MetadataPictureType.FrontCover;
            picture.MimeType = "image/jpeg";
            picture.Description = "desc";
            picture.Width = 300u;
            picture.Height = 300u;
            picture.Depth = 24u;
            picture.Colors = 0u;
            picture.Data = Encoding.ASCII.GetBytes("SOMEJPEGDATA");
            //picture.Length += picture.DataLength;

            unknown.IsLast = true;
            unknown.ID = new byte[] { 0xFE, 0xDC, 0xBA, 0x98 };
            unknown.Data = new byte[] { /*0xFE, 0xDC, 0xBA, 0x98, */0xF0, 0xE1, 0xD2, 0xC3 };
            //unknown.Length = 8;
        }

        public static void mutils__free_metadata_blocks(StreamInfo streamInfo, Padding padding, SeekTable seekTable, Application application1, Application application2, VorbisComment vorbisComment, CueSheet cueSheet, Picture picture, /*Unknown*/Application unknown)
        {
            seekTable.ResizePoints(0u);
            application1.SetData(null, false);
            //vorbisComment.VendorString = null;
            vorbisComment.ResizeComments(0u);
            cueSheet.ResizeIndices(0u, 0u);
            cueSheet.ResizeIndices(1u, 0u);
            cueSheet.ResizeTracks(0u);
            picture.MimeType = null;
            picture.Description = null;
            //picture.Data = null;
            unknown.Dispose();
        }

        public static bool file_utils__generate_flacfile(bool isOgg, [Pure]string outputFileName, ref long outputFileSize, uint length, StreamInfo streamInfo, Prototype[] metadata)
        {
            int[] samples = new int[1024];
            FlaCdotNet.Encoder.File encoder = new FlaCdotNet.Encoder.File();

            if (encoder == null)
            {
                return false;
            }

            encoder.SetOggSerialNumber(12345);
            encoder.SetVerify(true);
            encoder.SetStreamableSubset(true);
            encoder.SetDoMidSideStereo(false);
            encoder.SetLooseMidSideStereo(false);
            encoder.SetChannels(streamInfo.Channels);
            encoder.SetBitsPerSample(streamInfo.BitsPerSample);
            encoder.SetSampleRate(streamInfo.SampleRate);
            encoder.SetBlockSize(streamInfo.MinBlockSize);
            encoder.SetMaxLpcOrder(0u);
            encoder.SetQlpCoeffPrecision(0u);
            encoder.SetDoQlpCoeffPrecSearch(false);
            encoder.SetDoEscapeCoding(false);
            encoder.SetDoExhaustiveModelSearch(false);
            encoder.SetMinResidualPartitionOrder(0u);
            encoder.SetMaxResidualPartitionOrder(0u);
            encoder.SetRiceParameterSearchDist(0u);
            encoder.SetTotalSamplesEstimate(streamInfo.TotalSamples);
            encoder.SetMetadata(metadata);

            {
                var initStatus = isOgg ? encoder.InitOgg(outputFileName) : encoder.Init(outputFileName);
                if (initStatus != FlaCdotNet.Encoder.InitStatus.Ok)
                {
                    return false;
                }
            }

            for (var i = 0; i < samples.Length; i++)
            {
                samples[i] = i & 7;
            }

            uint n;
            while (length > 0)
            {
                n = Math.Min(length, (uint)samples.LongLength / 4);

                if (!encoder.ProcessInterleaved(samples, n))
                {
                    return false;
                }

                length -= n;
            }

            encoder.Finish();
            encoder.Dispose();

            if (outputFileSize != 0)
            {
                if (!File.Exists(outputFileName))
                {
                    return false;
                }
                else
                {
                    outputFileSize = new FileInfo(outputFileName).Length;
                }
            }

            return true;
        }
    }
}
