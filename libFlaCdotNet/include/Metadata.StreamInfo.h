// Metadata.StreamInfo.h

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
#include "stdafx.h"
#include "Metadata.Prototype.h"
using namespace System;
using namespace System::Diagnostics::Contracts;

namespace FlaCdotNet
{
	namespace Metadata
	{
		public ref class StreamInfo
			: public Prototype
		{
		protected:
			property FLAC::Metadata::StreamInfo* streamInfo_
			{
				FLAC::Metadata::StreamInfo* get();
			}
		public protected:
			StreamInfo(::FLAC__StreamMetadata* object, bool copy);
			StreamInfo^ Assign(::FLAC__StreamMetadata* object, bool copy);
		public:
			StreamInfo();

			property UInt32 MinBlockSize
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property UInt32 MaxBlockSize
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property UInt32 MinFrameSize
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property UInt32 MaxFrameSize
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

			property UInt32 BitsPerSample
			{
				UInt32 get();
				void set(UInt32 value);
			}

			property UInt64 TotalSamples
			{
				UInt64 get();
				void set(UInt64 value);
			}

			property array<Byte>^ MD5Sum
			{
				array<Byte>^ get();
				void set(array<Byte>^ value);
			}

			StreamInfo^ Clone();
			StreamInfo^ Assign(IntPtr object, bool copy);

			// Getters

			UInt32 GetMinBlockSize();
			UInt32 GetMaxBlockSize();
			UInt32 GetMinFrameSize();
			UInt32 GetMaxFrameSize();
			UInt32 GetSampleRate();
			UInt32 GetChannels();
			UInt32 GetBitsPerSample();
			UInt64 GetTotalSamples();
			array<Byte>^ GetMD5Sum();

			// Setters

			void SetMinBlockSize(UInt32 value);
			void SetMaxBlockSize(UInt32 value);
			void SetMinFrameSize(UInt32 value);
			void SetMaxFrameSize(UInt32 value);
			void SetSampleRate(UInt32 value);
			void SetChannels(UInt32 value);
			void SetBitsPerSample(UInt32 value);
			void SetTotalSamples(UInt64 value);
			void SetMD5Sum(array<Byte>^ value);
		};
	}
}
