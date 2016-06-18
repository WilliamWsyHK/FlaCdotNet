// Encoder.Stream.cpp

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

#include "Encoder.h"
using namespace System::Text;

namespace FlaCdotNet
{
	namespace Encoder
	{
		Stream::Stream()
#ifdef _MSC_VER
#pragma warning (disable: 4355)
#endif
			: encoder_(new EncoderFile(this, false))
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
			if (metadata_ != nullptr)
			{
				for (UInt64 i = 0ul; i < numMetadata_; i++)
				{
					delete metadata_[i];
				}
				delete[] metadata_;
				numMetadata_ = 0L;
				metadata_ = nullptr;
			}

			if (encoder_ != nullptr)
			{
				delete encoder_;
				encoder_ = nullptr;
			}
		}

		bool Stream::IsValid()
		{
			if (disposedValue)
			{
				throw gcnew ObjectDisposedException(this->GetType()->FullName);
			}
			return (encoder_ != nullptr) && encoder_->is_valid();
		}

		InitStatus Stream::Init()
		{
			return static_cast<InitStatus>(encoder_->init());
		}

		InitStatus Stream::InitOgg()
		{
			return static_cast<InitStatus>(encoder_->init_ogg());
		}

		bool Stream::Finish()
		{
			return encoder_->finish();
		}

		bool Stream::Process(array<Int32, 2>^ buffer, UInt32 samples)
		{
			Int32 channels = buffer->GetLength(0);
			Int32** b = new Int32*[channels];
			for (Int32 i = 0; i < channels; i++)
			{
				b[i] = new Int32[samples];
				for (Int64 j = 0L; j < samples; j++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					b[i][j] = buffer[i, j];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
			}

			bool result = encoder_->process(b, samples);

			for (Int32 i = 0; i < channels; i++)
			{
				if (b[i] != nullptr)
				{
					delete b[i];
				}
			}
			if (b != nullptr)
			{
				delete[] b;
			}
			return result;
		}

		bool Stream::ProcessInterleaved(array<Int32>^ buffer, UInt32 samples)
		{
			pin_ptr<Int32> p = &buffer[0];
			return encoder_->process_interleaved(p,
				//(buffer->Length / GetChannels())
				samples
			);
		}

		Stream::State^ Stream::GetState()
		{
			return gcnew Stream::State(encoder_->get_state());
		}

		Decoder::Stream::State^ Stream::GetVerifyDecoderState()
		{
			return gcnew Decoder::Stream::State(encoder_->get_verify_decoder_state());
		}

		void Stream::GetVerifyDecoderErrorStats(UInt64% absoluteSample, UInt32% frameNumber, UInt32% channel, UInt32% sample, Int32% expected, Int32% got)
		{
			pin_ptr<UInt64> a = &absoluteSample;
			pin_ptr<UInt32> f = &frameNumber;
			pin_ptr<UInt32> c = &channel;
			pin_ptr<UInt32> s = &sample;
			pin_ptr<Int32> e = &expected;
			pin_ptr<Int32> g = &got;
			return encoder_->get_verify_decoder_error_stats(a, f, c, s, e, g);
		}

		bool Stream::GetVerify()
		{
			return encoder_->get_verify();
		}

		bool Stream::GetStreamableSubset()
		{
			return encoder_->get_streamable_subset();
		}

		bool Stream::GetDoMidSideStereo()
		{
			return encoder_->get_do_mid_side_stereo();
		}

		bool Stream::GetLooseMidSideStereo()
		{
			return encoder_->get_loose_mid_side_stereo();
		}

		UInt32 Stream::GetChannels()
		{
			return encoder_->get_channels();
		}

		UInt32 Stream::GetBitsPerSample()
		{
			return encoder_->get_bits_per_sample();
		}

		UInt32 Stream::GetSampleRate()
		{
			return encoder_->get_sample_rate();
		}

		UInt32 Stream::GetBlockSize()
		{
			return encoder_->get_blocksize();
		}

		UInt32 Stream::GetMaxLpcOrder()
		{
			return encoder_->get_max_lpc_order();
		}

		UInt32 Stream::GetQlpCoeffPrecision()
		{
			return encoder_->get_qlp_coeff_precision();
		}

		bool Stream::GetDoQlpCoeffPrecSearch()
		{
			return encoder_->get_do_qlp_coeff_prec_search();
		}

		bool Stream::GetDoEscapeCoding()
		{
			return encoder_->get_do_escape_coding();
		}

		bool Stream::GetDoExhaustiveModelSearch()
		{
			return encoder_->get_do_exhaustive_model_search();
		}

		UInt32 Stream::GetMinResidualPartitionOrder()
		{
			return encoder_->get_min_residual_partition_order();
		}

		UInt32 Stream::GetMaxResidualPartitionOrder()
		{
			return encoder_->get_max_residual_partition_order();
		}

		UInt32 Stream::GetRiceParameterSearchDist()
		{
			return encoder_->get_rice_parameter_search_dist();
		}

		UInt64 Stream::GetTotalSamplesEstimate()
		{
			return encoder_->get_total_samples_estimate();
		}

		bool Stream::SetOggSerialNumber(Int32 value)
		{
			return encoder_->set_ogg_serial_number(value);
		}

		bool Stream::SetVerify(bool value)
		{
			return encoder_->set_verify(value);
		}

		bool Stream::SetStreamableSubset(bool value)
		{
			return encoder_->set_streamable_subset(value);
		}

		bool Stream::SetChannels(UInt32 value)
		{
			return encoder_->set_channels(value);
		}

		bool Stream::SetBitsPerSample(UInt32 value)
		{
			return encoder_->set_bits_per_sample(value);
		}

		bool Stream::SetSampleRate(UInt32 value)
		{
			return encoder_->set_sample_rate(value);
		}

		bool Stream::SetCompressionLevel(UInt32 value)
		{
			return encoder_->set_compression_level(value);
		}

		bool Stream::SetBlockSize(UInt32 value)
		{
			return encoder_->set_blocksize(value);
		}

		bool Stream::SetDoMidSideStereo(bool value)
		{
			return encoder_->set_do_mid_side_stereo(value);
		}

		bool Stream::SetLooseMidSideStereo(bool value)
		{
			return encoder_->set_loose_mid_side_stereo(value);
		}

		bool Stream::SetApodization(String^ specification)
		{
			array<Byte>^ asciiBytes = Encoding::ASCII->GetBytes(specification + "\0");
			pin_ptr<Byte> p = &asciiBytes[0];
			return encoder_->set_apodization(reinterpret_cast<const char*>(p));
		}

		bool Stream::SetMaxLpcOrder(UInt32 value)
		{
			return encoder_->set_max_lpc_order(value);
		}

		bool Stream::SetQlpCoeffPrecision(UInt32 value)
		{
			return encoder_->set_qlp_coeff_precision(value);
		}

		bool Stream::SetDoQlpCoeffPrecSearch(bool value)
		{
			return encoder_->set_do_qlp_coeff_prec_search(value);
		}

		bool Stream::SetDoEscapeCoding(bool value)
		{
			return encoder_->set_do_escape_coding(value);
		}

		bool Stream::SetDoExhaustiveModelSearch(bool value)
		{
			return encoder_->set_do_exhaustive_model_search(value);
		}

		bool Stream::SetMinResidualPartitionOrder(UInt32 value)
		{
			return encoder_->set_min_residual_partition_order(value);
		}

		bool Stream::SetMaxResidualPartitionOrder(UInt32 value)
		{
			return encoder_->set_max_residual_partition_order(value);
		}

		bool Stream::SetRiceParameterSearchDist(UInt32 value)
		{
			return encoder_->set_rice_parameter_search_dist(value);
		}

		bool Stream::SetTotalSamplesEstimate(UInt64 value)
		{
			return encoder_->set_total_samples_estimate(value);
		}

		bool Stream::SetMetadata(array<Metadata::Prototype^>^ metadata)
		{
			if (metadata_ == nullptr)
			{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
				metadata_ = new FLAC::Metadata::Prototype*[metadata->LongLength];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
			}
			else
			{
				for (UInt64 i = 0ul; i < numMetadata_; i++)
				{
					delete metadata_[i];
				}
				if (numMetadata_ != metadata->LongLength)
				{
					delete[] metadata_;
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					metadata_ = new FLAC::Metadata::Prototype*[metadata->LongLength];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
			}
			numMetadata_ = metadata->LongLength;

			for (UInt64 i = 0ul; i < numMetadata_; i++)
			{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
				metadata_[i] = FLAC::Metadata::clone(metadata[i]->prototype_);
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
			}

#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
			bool result = encoder_->set_metadata(metadata_, numMetadata_);
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif

			return result;
		}

		ReadStatus Stream::readCallback(array<Byte>^ buffer, size_t% bytes)
		{
			return ReadStatus::Unsupported;
		}

		SeekStatus Stream::seekCallback(UInt64 absoluteByteOffset)
		{
			return SeekStatus::Unsupported;
		}

		TellStatus Stream::tellCallback(UInt64% absoluteByteOffset)
		{
			return TellStatus::Unsupported;
		}

		void Stream::metadataCallback(Metadata::Prototype^ metadata)
		{
		}
	}
}
