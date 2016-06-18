// Metadata.StreamInfo.cpp

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

#include "Metadata.StreamInfo.h"
using namespace System::Runtime::InteropServices;

namespace FlaCdotNet
{
	namespace Metadata
	{
		StreamInfo::StreamInfo()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: StreamInfo(::FLAC__metadata_object_new(::FLAC__MetadataType::FLAC__METADATA_TYPE_STREAMINFO), false)
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			prototype_ = new FLAC::Metadata::StreamInfo(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_STREAMINFO), false);
			selfConstruct = true;
#ifdef CONTRACTS_FULL
			Contract::Ensures(prototype_ != nullptr);
#else
			if (!(prototype_ != nullptr))
			{
				throw gcnew OutOfMemoryException();
			}
#endif
#endif
#endif
		}

		StreamInfo::StreamInfo(::FLAC__StreamMetadata* object, bool copy)
			: Prototype()
		{
			prototype_ = new FLAC::Metadata::StreamInfo(object, copy);
			selfConstruct = true;
#ifdef CONTRACTS_FULL
			Contract::Ensures(prototype_ != nullptr);
#else
			if (!(prototype_ != nullptr))
			{
				throw gcnew OutOfMemoryException();
			}
#endif
		}

		FLAC::Metadata::StreamInfo* StreamInfo::streamInfo_::get()
		{
			return dynamic_cast<FLAC::Metadata::StreamInfo*>(prototype_);
		}

		UInt32 StreamInfo::MinBlockSize::get()
		{
			return GetMinBlockSize();
		}

		void StreamInfo::MinBlockSize::set(UInt32 value)
		{
			SetMinBlockSize(value);
		}

		UInt32 StreamInfo::MaxBlockSize::get()
		{
			return GetMaxBlockSize();
		}

		void StreamInfo::MaxBlockSize::set(UInt32 value)
		{
			SetMaxBlockSize(value);
		}

		UInt32 StreamInfo::MinFrameSize::get()
		{
			return GetMinFrameSize();
		}

		void StreamInfo::MinFrameSize::set(UInt32 value)
		{
			SetMinFrameSize(value);
		}

		UInt32 StreamInfo::MaxFrameSize::get()
		{
			return GetMaxFrameSize();
		}

		void StreamInfo::MaxFrameSize::set(UInt32 value)
		{
			SetMaxFrameSize(value);
		}

		UInt32 StreamInfo::SampleRate::get()
		{
			return GetSampleRate();
		}

		void StreamInfo::SampleRate::set(UInt32 value)
		{
			SetSampleRate(value);
		}

		UInt32 StreamInfo::Channels::get()
		{
			return GetChannels();
		}

		void StreamInfo::Channels::set(UInt32 value)
		{
			SetChannels(value);
		}

		UInt32 StreamInfo::BitsPerSample::get()
		{
			return GetBitsPerSample();
		}

		void StreamInfo::BitsPerSample::set(UInt32 value)
		{
			SetBitsPerSample(value);
		}

		UInt64 StreamInfo::TotalSamples::get()
		{
			return GetTotalSamples();
		}

		void StreamInfo::TotalSamples::set(UInt64 value)
		{
			SetTotalSamples(value);
		}

		array<Byte>^ StreamInfo::MD5Sum::get()
		{
			return GetMD5Sum();
		}

		void StreamInfo::MD5Sum::set(array<Byte>^ value)
		{
			SetMD5Sum(value);
		}

		StreamInfo^ StreamInfo::Clone()
		{
			return dynamic_cast<StreamInfo^>(Prototype::Clone());
		}

		StreamInfo^ StreamInfo::Assign(::FLAC__StreamMetadata* object, bool copy)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			streamInfo_->assign(object, copy);
			return this;
		}

		StreamInfo^ StreamInfo::Assign(IntPtr object, bool copy)
		{
			return Assign(static_cast<::FLAC__StreamMetadata*>(object.ToPointer()), copy);
		}

		UInt32 StreamInfo::GetMinBlockSize()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return streamInfo_->get_min_blocksize();
		}

		UInt32 StreamInfo::GetMaxBlockSize()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return streamInfo_->get_max_blocksize();
		}

		UInt32 StreamInfo::GetMinFrameSize()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return streamInfo_->get_min_framesize();
		}

		UInt32 StreamInfo::GetMaxFrameSize()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return streamInfo_->get_max_framesize();
		}

		UInt32 StreamInfo::GetSampleRate()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return streamInfo_->get_sample_rate();
		}

		UInt32 StreamInfo::GetChannels()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return streamInfo_->get_channels();
		}

		UInt32 StreamInfo::GetBitsPerSample()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return streamInfo_->get_bits_per_sample();
		}

		UInt64 StreamInfo::GetTotalSamples()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return streamInfo_->get_total_samples();
		}

		array<Byte>^ StreamInfo::GetMD5Sum()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			array<Byte> ^a = gcnew array<Byte>(16);
			Marshal::Copy(IntPtr(const_cast<Byte*>(streamInfo_->get_md5sum())), a, 0, 16);
			return a;
		}

		void StreamInfo::SetMinBlockSize(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			streamInfo_->set_min_blocksize(value);
		}

		void StreamInfo::SetMaxBlockSize(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			streamInfo_->set_max_blocksize(value);
		}

		void StreamInfo::SetMinFrameSize(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			streamInfo_->set_min_framesize(value);
		}

		void StreamInfo::SetMaxFrameSize(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			streamInfo_->set_max_framesize(value);
		}

		void StreamInfo::SetSampleRate(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			streamInfo_->set_sample_rate(value);
		}

		void StreamInfo::SetChannels(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			streamInfo_->set_channels(value);
		}

		void StreamInfo::SetBitsPerSample(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			streamInfo_->set_bits_per_sample(value);
		}

		void StreamInfo::SetTotalSamples(UInt64 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			streamInfo_->set_total_samples(value);
		}

		void StreamInfo::SetMD5Sum(array<Byte>^ value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
#ifdef CONTRACTS_FULL
			Contract::Requires(value->Length <= 16);
#else
			if (!(value->Length <= 16))
			{
				throw gcnew ArgumentException("value.Length must be <= 16");
			}
#endif
			pin_ptr<Byte> m = &value[0];
			streamInfo_->set_md5sum(m);
		}
	}
}
