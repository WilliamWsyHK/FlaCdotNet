// Metadata.SeekTable.SeekPoint.cpp

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

#include "Stdafx.h"

#include "Metadata.SeekTable.h"

namespace FlaCdotNet
{
	namespace Metadata
	{
		SeekTable::SeekPoint::SeekPoint()
			: point_(new ::FLAC__StreamMetadata_SeekPoint()),
			selfConstruct(true)
		{
		}

		SeekTable::SeekPoint::SeekPoint(::FLAC__StreamMetadata_SeekPoint object)
			: point_(new ::FLAC__StreamMetadata_SeekPoint(object))
		{
		}

		SeekTable::SeekPoint::SeekPoint(IntPtr object)
			: point_(static_cast<::FLAC__StreamMetadata_SeekPoint*>(object.ToPointer()))
		{
		}

		SeekTable::SeekPoint::~SeekPoint()
		{
			if (!disposedValue)
			{
				this->!SeekPoint();

				point_ = nullptr;

				disposedValue = true;
			}
		}

		SeekTable::SeekPoint::!SeekPoint()
		{
			if (selfConstruct)
			{
				delete point_;
			}
		}

		UInt64 SeekTable::SeekPoint::SampleNumber::get()
		{
			return GetSampleNumber();
		}

		void SeekTable::SeekPoint::SampleNumber::set(UInt64 value)
		{
			SetSampleNumber(value);
		}

		UInt64 SeekTable::SeekPoint::StreamOffset::get()
		{
			return GetStreamOffset();
		}

		void SeekTable::SeekPoint::StreamOffset::set(UInt64 value)
		{
			SetStreamOffset(value);
		}

		UInt32 SeekTable::SeekPoint::FrameSamples::get()
		{
			return GetFrameSamples();
		}

		void SeekTable::SeekPoint::FrameSamples::set(UInt32 value)
		{
			SetFrameSamples(value);
		}

		bool SeekTable::SeekPoint::IsValid()
		{
			if (disposedValue)
			{
				throw gcnew ObjectDisposedException(this->GetType()->FullName);
			}
			return (point_ != nullptr);
		}

		UInt64 SeekTable::SeekPoint::GetSampleNumber()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return point_->sample_number;
		}

		UInt64 SeekTable::SeekPoint::GetStreamOffset()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return point_->stream_offset;
		}

		UInt32 SeekTable::SeekPoint::GetFrameSamples()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return point_->frame_samples;
		}

		void SeekTable::SeekPoint::SetSampleNumber(UInt64 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			point_->sample_number = value;
		}

		void SeekTable::SeekPoint::SetStreamOffset(UInt64 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			point_->stream_offset = value;
		}

		void SeekTable::SeekPoint::SetFrameSamples(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			point_->frame_samples = value;
		}
	}
}
