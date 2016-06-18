// Decoder.Stream.cpp

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

#include "Decoder.h"

namespace FlaCdotNet
{
	namespace Decoder
	{
		Stream::Stream()
#ifdef _MSC_VER
#pragma warning (disable: 4355)
#endif
			: decoder_(new DecoderFile(this, false))
#ifdef _MSC_VER
#pragma warning (default: 4355)
#endif
		{
		}

		Stream::~Stream()
		{
			if (!disposedValue)
			{
				this->!Stream();

				disposedValue = true;
			}
		}

		Stream::!Stream()
		{
			if (decoder_ != nullptr)
			{
				delete decoder_;
				decoder_ = nullptr;
			}
		}

		bool Stream::IsValid()
		{
			if (disposedValue)
			{
				throw gcnew ObjectDisposedException(this->GetType()->FullName);
			}
			return (decoder_ != nullptr) && decoder_->is_valid();
		}

		InitStatus Stream::Init()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return static_cast<InitStatus>(decoder_->init());
		}

		InitStatus Stream::InitOgg()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return static_cast<InitStatus>(decoder_->init_ogg());
		}

		bool Stream::Finish()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return decoder_->finish();
		}

		bool Stream::Flush()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return decoder_->flush();
		}

		bool Stream::Reset()
		{
			return decoder_->reset();
		}

		bool Stream::ProcessSingle()
		{
			return decoder_->process_single();
		}

		bool Stream::ProcessUntilEndOfMetadata()
		{
			return decoder_->process_until_end_of_metadata();
		}

		bool Stream::ProcessUntilEndOfStream()
		{
			return decoder_->process_until_end_of_stream();
		}

		bool Stream::SkipSingleFrame()
		{
			return decoder_->skip_single_frame();
		}

		bool Stream::SeekAbsolute(UInt64 sample)
		{
			return decoder_->seek_absolute(sample);
		}

		Stream::State^ Stream::GetState()
		{
			return gcnew Stream::State(decoder_->get_state());
		}

		bool Stream::GetMD5Checking()
		{
			return decoder_->get_md5_checking();
		}

		UInt64 Stream::GetTotalSamples()
		{
			return decoder_->get_total_samples();
		}

		UInt32 Stream::GetChannels()
		{
			return decoder_->get_channels();
		}

		FlaCdotNet::ChannelAssignment Stream::GetChannelAssignment()
		{
			return static_cast<FlaCdotNet::ChannelAssignment>(decoder_->get_channel_assignment());
		}

		UInt32 Stream::GetBitsPerSample()
		{
			return decoder_->get_bits_per_sample();
		}

		UInt32 Stream::GetSampleRate()
		{
			return decoder_->get_sample_rate();
		}

		UInt32 Stream::GetBlockSize()
		{
			return decoder_->get_blocksize();
		}

		bool Stream::GetDecodePosition(UInt64% position)
		{
			pin_ptr<UInt64> p = &position;
			return decoder_->get_decode_position(p);
		}

		bool Stream::SetOggSerialNumber(Int32 value)
		{
			return decoder_->set_ogg_serial_number(value);
		}

		bool Stream::SetMD5Checking(bool value)
		{
			return decoder_->set_md5_checking(value);
		}

		bool Stream::SetMetadataRespond(FlaCdotNet::MetadataType type)
		{
			return decoder_->set_metadata_respond(static_cast<::FLAC__MetadataType>(type));
		}

		bool Stream::SetMetadataRespondApplication(array<Byte>^ id)
		{
			if (id->Length > 4)
			{
				throw gcnew ArgumentOutOfRangeException("id", "Length cannot be greater than 4");
			}
			pin_ptr<Byte> p = &id[0];
			return decoder_->set_metadata_respond_application(p);
		}

		bool Stream::SetMetadataRespondAll()
		{
			return decoder_->set_metadata_respond_all();
		}

		bool Stream::SetMetadataIgnore(FlaCdotNet::MetadataType type)
		{
			return decoder_->set_metadata_ignore(static_cast<::FLAC__MetadataType>(type));
		}

		bool Stream::SetMetadataIgnoreApplication(array<Byte>^ id)
		{
			if (id->Length > 4)
			{
				throw gcnew ArgumentOutOfRangeException("id", "Length cannot be greater than 4");
			}
			pin_ptr<Byte> p = &id[0];
			return decoder_->set_metadata_ignore_application(p);
		}

		bool Stream::SetMetadataIgnoreAll()
		{
			return decoder_->set_metadata_ignore_all();
		}

		SeekStatus Stream::seekCallback(UInt64 absoluteByteOffset)
		{
			return static_cast<SeekStatus>(::FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED);
		}

		TellStatus Stream::tellCallback(UInt64% absoluteByteOffset)
		{
			return static_cast<TellStatus>(::FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED);
		}

		LengthStatus Stream::lengthCallback(UInt64% streamLength)
		{
			return static_cast<LengthStatus>(::FLAC__STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED);
		}

		bool Stream::eofCallback()
		{
			return false;
		}

		void Stream::metadataCallback(FlaCdotNet::Metadata::Prototype^ metadata)
		{
		}
	}
}
