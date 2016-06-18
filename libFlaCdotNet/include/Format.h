// Format.h

/* libFlaCdotNet - Wrapper of libFLAC++ for .NET Environment
* Copyright (C) 2016-      WONG, William Sung Yu
* =============================================
* libFLAC++ - Free Lossless Audio Codec library
* Copyright (C) 2002-2009  Josh Coalson
* Copyright (C) 2011-2014  Xiph.Org Foundation
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* - Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
*
* - Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* - Neither the name of the Xiph.org Foundation nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#include "Stdafx.h"
using namespace System;

namespace FlaCdotNet
{
	public ref class Constants
	{
	public:
		static String^ VendorString = "reference libFlaCdotNet 1.0.0 (Wrapping libFLAC++ 1.3.1) 20160619";
	};

	public enum class MetadataType
	{
		StreamInfo      = ::FLAC__METADATA_TYPE_STREAMINFO,
		Padding         = ::FLAC__METADATA_TYPE_PADDING,
		Application     = ::FLAC__METADATA_TYPE_APPLICATION,
		SeekTable       = ::FLAC__METADATA_TYPE_SEEKTABLE,
		VorbisComment   = ::FLAC__METADATA_TYPE_VORBIS_COMMENT,
		CueSheet        = ::FLAC__METADATA_TYPE_CUESHEET,
		Picture         = ::FLAC__METADATA_TYPE_PICTURE,
		Undefined       = ::FLAC__METADATA_TYPE_UNDEFINED,
		MaxMetadataType = ::FLAC__MAX_METADATA_TYPE
	};

	public enum class MetadataPictureType
	{
		Other              = ::FLAC__STREAM_METADATA_PICTURE_TYPE_OTHER,
		FileIconStandard   = ::FLAC__STREAM_METADATA_PICTURE_TYPE_FILE_ICON_STANDARD,
		FileIcon           = ::FLAC__STREAM_METADATA_PICTURE_TYPE_FILE_ICON,
		FrontCover         = ::FLAC__STREAM_METADATA_PICTURE_TYPE_FRONT_COVER,
		BackCover          = ::FLAC__STREAM_METADATA_PICTURE_TYPE_BACK_COVER,
		LeafletPage        = ::FLAC__STREAM_METADATA_PICTURE_TYPE_LEAFLET_PAGE,
		Media              = ::FLAC__STREAM_METADATA_PICTURE_TYPE_MEDIA,
		LeadArtist         = ::FLAC__STREAM_METADATA_PICTURE_TYPE_LEAD_ARTIST,
		Artist             = ::FLAC__STREAM_METADATA_PICTURE_TYPE_ARTIST,
		Conductor          = ::FLAC__STREAM_METADATA_PICTURE_TYPE_CONDUCTOR,
		Band               = ::FLAC__STREAM_METADATA_PICTURE_TYPE_BAND,
		Composer           = ::FLAC__STREAM_METADATA_PICTURE_TYPE_COMPOSER,
		Lyricist           = ::FLAC__STREAM_METADATA_PICTURE_TYPE_LYRICIST,
		RecordingLocation  = ::FLAC__STREAM_METADATA_PICTURE_TYPE_RECORDING_LOCATION,
		DuringRecording    = ::FLAC__STREAM_METADATA_PICTURE_TYPE_DURING_RECORDING,
		DuringPerformance  = ::FLAC__STREAM_METADATA_PICTURE_TYPE_DURING_PERFORMANCE,
		VideoScreenCapture = ::FLAC__STREAM_METADATA_PICTURE_TYPE_VIDEO_SCREEN_CAPTURE,
		Fish               = ::FLAC__STREAM_METADATA_PICTURE_TYPE_FISH,
		Illustration       = ::FLAC__STREAM_METADATA_PICTURE_TYPE_ILLUSTRATION,
		BandLogoType       = ::FLAC__STREAM_METADATA_PICTURE_TYPE_BAND_LOGOTYPE,
		PublisherLogoType  = ::FLAC__STREAM_METADATA_PICTURE_TYPE_PUBLISHER_LOGOTYPE
	};

	public enum class ChannelAssignment
	{
		Independent = ::FLAC__CHANNEL_ASSIGNMENT_INDEPENDENT,
		LeftSide    = ::FLAC__CHANNEL_ASSIGNMENT_LEFT_SIDE,
		RightSide   = ::FLAC__CHANNEL_ASSIGNMENT_RIGHT_SIDE,
		MidSide     = ::FLAC__CHANNEL_ASSIGNMENT_MID_SIDE
	};

	public enum class FrameNumberType
	{
		FrameNumber  = ::FLAC__FRAME_NUMBER_TYPE_FRAME_NUMBER,
		SampleNumber = ::FLAC__FRAME_NUMBER_TYPE_SAMPLE_NUMBER
	};

	public enum class SubframeType
	{
		Constant = ::FLAC__SUBFRAME_TYPE_CONSTANT,
		Verbatim = ::FLAC__SUBFRAME_TYPE_VERBATIM,
		Fixed    = ::FLAC__SUBFRAME_TYPE_FIXED,
		Lpc      = ::FLAC__SUBFRAME_TYPE_LPC
	};

	public enum class EntropyCodingMethodType
	{
		PartitionedRice  = ::FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE,
		PartitionedRice2 = ::FLAC__ENTROPY_CODING_METHOD_PARTITIONED_RICE2
	};

	public ref class EntropyCodingMethod
	{
	private:
	internal:
		EntropyCodingMethod(FLAC__EntropyCodingMethod* entropyCodingMethod);
	protected:
		FLAC__EntropyCodingMethod* entropyCodingMethod_;
	public:
		ref class EntropyCodingMethodPartitionedRice
		{
		private:
		internal:
			EntropyCodingMethodPartitionedRice(FLAC__EntropyCodingMethod_PartitionedRice* partitionedRice);
		protected:
			FLAC__EntropyCodingMethod_PartitionedRice* partitionedRice_;
		public:
			ref class PartitionedRiceContents
			{
			private:
			internal:
				PartitionedRiceContents(FLAC__EntropyCodingMethod_PartitionedRiceContents* contents);
			protected:
				FLAC__EntropyCodingMethod_PartitionedRiceContents* contents_;
			public:
				// To be implemented
			};

			property UInt32 Order
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property array<PartitionedRiceContents^>^ Contents
			{
				array<PartitionedRiceContents^>^ get();
			}
		protected:
			array<PartitionedRiceContents^>^ contents_;
		};

		property EntropyCodingMethodType EntropyCodingMethodType
		{
			FlaCdotNet::EntropyCodingMethodType get();
			void set(FlaCdotNet::EntropyCodingMethodType value);
		}

		property EntropyCodingMethodPartitionedRice^ PartitionedRice
		{
			EntropyCodingMethodPartitionedRice^ get();
		}
	protected:
		initonly EntropyCodingMethodPartitionedRice^ partitionedRice_;
	};

	public ref class Frame
	{
	private:
		// Finalizer
		!Frame();

		void createSubframes();
	internal:
		Frame(const ::FLAC__Frame* frame);
	public protected:
		::FLAC__Frame* frame_;
	public:
		ref class FrameHeader
		{
		private:
		internal:
			FrameHeader(::FLAC__FrameHeader* frameHeader);
		protected:
			::FLAC__FrameHeader* frameHeader_;
		public:
			property UInt32 BlockSize
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property UInt32 SampleRate
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property UInt32 Channels
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property ChannelAssignment ChannelAssignment
			{
				FlaCdotNet::ChannelAssignment get();
				void set(FlaCdotNet::ChannelAssignment value);
			}

			property UInt32 BitsPerSample
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property FrameNumberType NumberType
			{
				FrameNumberType get();
			}

			property UInt32 FrameNumber
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property UInt64 SampleNumber
			{
				UInt64 get();
				void set(UInt64 value);
			}

			property Byte Crc
			{
				Byte get();
			}
		};

		ref class Subframe
		{
		private:
		internal:
			Subframe(::FLAC__Subframe* subframe);
		protected:
			::FLAC__Subframe* subframe_;
		public:
			ref class SubframeConstant
			{
			private:
			internal:
				SubframeConstant(FLAC__Subframe_Constant* constant);
			protected:
				FLAC__Subframe_Constant* constant_;
			public:
				property Int32 Value
				{
					Int32 get();
					void set(Int32 value);
				}
			};

			ref class SubframeFixed
			{
			private:
			internal:
				SubframeFixed(FLAC__Subframe_Fixed* fixed);
			protected:
				FLAC__Subframe_Fixed* fixed_;
				initonly EntropyCodingMethod^ entropyCodingMethod_;
			public:
				property EntropyCodingMethod^ EntropyCodingMethod
				{
					FlaCdotNet::EntropyCodingMethod^ get();
				}

				property UInt32 Order
				{
					UInt32 get();
					void set(UInt32 value);
				}

				property array<Int32>^ Warmup
				{
					array<Int32>^ get();
				}

				// Residue
			};

			ref class SubframeLpc
			{
			private:
			internal:
				SubframeLpc(FLAC__Subframe_LPC* lpc);
			protected:
				FLAC__Subframe_LPC* lpc_;
				initonly EntropyCodingMethod^ entropyCodingMethod_;
			public:
				property EntropyCodingMethod^ EntropyCodingMethod
				{
					FlaCdotNet::EntropyCodingMethod^ get();
				}

				property UInt32 Order
				{
					UInt32 get();
					void set(UInt32 value);
				}

				property UInt32 QlpCoeffPrecision
				{
					UInt32 get();
					void set(UInt32 value);
				}

				property Int32 QuantizationLevel
				{
					Int32 get();
					void set(Int32 value);
				}

				// To be implemented
				// QlpCoeff, Warmup, Residual
			};

			ref class SubframeVerbatim
			{
			private:
			internal:
				SubframeVerbatim(FLAC__Subframe_Verbatim* verbatim);
			protected:
				FLAC__Subframe_Verbatim* verbatim_;
			public:
				// To be implemented
				// Data
			};

			property SubframeType SubframeType
			{
				FlaCdotNet::SubframeType get();
				void set(FlaCdotNet::SubframeType value);
			}

			property SubframeConstant^ Constant
			{
				SubframeConstant^ get();
			}

			property SubframeFixed^ Fixed
			{
				SubframeFixed^ get();
			}

			property SubframeLpc^ Lpc
			{
				SubframeLpc^ get();
			}

			property SubframeVerbatim^ Verbatim
			{
				SubframeVerbatim^ get();
			}
		protected:
			initonly SubframeConstant^ constant_;
			initonly SubframeFixed^ fixed_;
			initonly SubframeLpc^ lpc_;
			initonly SubframeVerbatim^ verbatim_;
		};

		ref class FrameFooter
		{
		private:
		internal:
			FrameFooter(::FLAC__FrameFooter* frameFooter);
		protected:
			::FLAC__FrameFooter* frameFooter_;
		public:
			property UInt16 Crc
			{
				UInt16 get();
			}
		};

		// Dispose
		~Frame();

		property FrameHeader^ Header
		{
			FrameHeader^ get();
		}

		property array<Subframe^>^ Subframes
		{
			array<Subframe^>^ get();
		}

		property Subframe^ default[Byte]
		{
			Subframe^ get(Byte index);
		}

		property FrameFooter^ Footer
		{
			FrameFooter^ get();
		}
	protected:
		FrameHeader^ header_;
		array<Subframe^>^ subframes_;
		FrameFooter^ footer_;
	};
}