// Format.Frame.cpp

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

#include "Format.h"

namespace FlaCdotNet
{
	Frame::Frame(const ::FLAC__Frame* frame)
	{
		frame_ = const_cast<::FLAC__Frame*>(frame);
	}

	Frame::~Frame()
	{
		this->!Frame();
	}

	Frame::!Frame()
	{
		if (frame_ != nullptr)
		{
			frame_ = nullptr;
		}
	}

	Frame::FrameHeader^ Frame::Header::get()
	{
		if (header_ == nullptr)
		{
			header_ = gcnew FrameHeader(&(frame_->header));
		}
		return header_;
	}

	array<Frame::Subframe^>^ Frame::Subframes::get()
	{
		createSubframes();
		return subframes_;
	}

	Frame::Subframe^ Frame::default::get(Byte index)
	{
		createSubframes();
		return subframes_[index];
	}

	Frame::FrameFooter^ Frame::Footer::get()
	{
		if (footer_ == nullptr)
		{
			footer_ = gcnew FrameFooter(&(frame_->footer));
		}
		return footer_;
	}

	void Frame::createSubframes()
	{
		if (subframes_ != nullptr)
		{
			return;
		}
		subframes_ = gcnew array<Subframe^>(FLAC__MAX_CHANNELS);
		for (Byte i = 0u; i < FLAC__MAX_CHANNELS; i++)
		{
			subframes_[i] = gcnew Subframe(&(frame_->subframes[i]));
		}
	}

	// ---------------------------

	Frame::FrameHeader::FrameHeader(::FLAC__FrameHeader* frameHeader)
	{
		frameHeader_ = frameHeader;
	}

	UInt32 Frame::FrameHeader::BlockSize::get()
	{
		return frameHeader_->blocksize;
	}

	void Frame::FrameHeader::BlockSize::set(UInt32 value)
	{
		frameHeader_->blocksize = value;
	}

	UInt32 Frame::FrameHeader::SampleRate::get()
	{
		return frameHeader_->sample_rate;
	}

	void Frame::FrameHeader::SampleRate::set(UInt32 value)
	{
		frameHeader_->sample_rate = value;
	}

	UInt32 Frame::FrameHeader::Channels::get()
	{
		return frameHeader_->channels;
	}

	void Frame::FrameHeader::Channels::set(UInt32 value)
	{
		frameHeader_->channels = value;
	}

	ChannelAssignment Frame::FrameHeader::ChannelAssignment::get()
	{
		return static_cast<FlaCdotNet::ChannelAssignment>(frameHeader_->channel_assignment);
	}

	void Frame::FrameHeader::ChannelAssignment::set(FlaCdotNet::ChannelAssignment value)
	{
		frameHeader_->channel_assignment = static_cast<::FLAC__ChannelAssignment>(value);
	}

	UInt32 Frame::FrameHeader::BitsPerSample::get()
	{
		return frameHeader_->bits_per_sample;
	}

	void Frame::FrameHeader::BitsPerSample::set(UInt32 value)
	{
		frameHeader_->bits_per_sample = value;
	}

	FrameNumberType Frame::FrameHeader::NumberType::get()
	{
		return static_cast<FrameNumberType>(frameHeader_->number_type);
	}

	UInt32 Frame::FrameHeader::FrameNumber::get()
	{
		return frameHeader_->number.frame_number;
	}

	void Frame::FrameHeader::FrameNumber::set(UInt32 value)
	{
		frameHeader_->number.frame_number = value;
	}

	UInt64 Frame::FrameHeader::SampleNumber::get()
	{
		return frameHeader_->number.sample_number;
	}

	void Frame::FrameHeader::SampleNumber::set(UInt64 value)
	{
		frameHeader_->number.sample_number = value;
	}

	Byte Frame::FrameHeader::Crc::get()
	{
		return frameHeader_->crc;
	}

	// ---------------------------

	Frame::Subframe::Subframe(::FLAC__Subframe* subframe)
		: subframe_(subframe)
	{
		switch (SubframeType)
		{
		case FlaCdotNet::SubframeType::Constant:
			constant_ = gcnew SubframeConstant(&(subframe_->data.constant));
			break;
		case FlaCdotNet::SubframeType::Fixed:
			fixed_ = gcnew SubframeFixed(&(subframe_->data.fixed));
			break;
		case FlaCdotNet::SubframeType::Lpc:
			lpc_ = gcnew SubframeLpc(&(subframe_->data.lpc));
			break;
		case FlaCdotNet::SubframeType::Verbatim:
			verbatim_ = gcnew SubframeVerbatim(&(subframe_->data.verbatim));
			break;
		default:
			break;
		}
	}

	SubframeType Frame::Subframe::SubframeType::get()
	{
		return static_cast<FlaCdotNet::SubframeType>(subframe_->type);
	}

	void Frame::Subframe::SubframeType::set(FlaCdotNet::SubframeType value)
	{
		subframe_->type = static_cast<FLAC__SubframeType>(value);
	}

	Frame::Subframe::SubframeConstant::SubframeConstant(FLAC__Subframe_Constant* constant)
		: constant_(constant)
	{
	}

	Int32 Frame::Subframe::SubframeConstant::Value::get()
	{
		return constant_->value;
	}

	void Frame::Subframe::SubframeConstant::Value::set(Int32 value)
	{
		constant_->value = value;
	}

	Frame::Subframe::SubframeFixed::SubframeFixed(FLAC__Subframe_Fixed* fixed)
		: fixed_(fixed)
	{
		entropyCodingMethod_ = gcnew FlaCdotNet::EntropyCodingMethod(&(fixed_->entropy_coding_method));
	}

	EntropyCodingMethod^ Frame::Subframe::SubframeFixed::EntropyCodingMethod::get()
	{
		return entropyCodingMethod_;
	}

	UInt32 Frame::Subframe::SubframeFixed::Order::get()
	{
		return fixed_->order;
	}

	void Frame::Subframe::SubframeFixed::Order::set(UInt32 value)
	{
		fixed_->order = value;
	}

	array<Int32>^ Frame::Subframe::SubframeFixed::Warmup::get()
	{
		// TODO: implement the real method
		throw gcnew NotImplementedException();
	}

	Frame::Subframe::SubframeLpc::SubframeLpc(FLAC__Subframe_LPC* lpc)
		: lpc_(lpc)
	{
		entropyCodingMethod_ = gcnew FlaCdotNet::EntropyCodingMethod(&(lpc_->entropy_coding_method));
	}

	EntropyCodingMethod^ Frame::Subframe::SubframeLpc::EntropyCodingMethod::get()
	{
		return entropyCodingMethod_;
	}

	UInt32 Frame::Subframe::SubframeLpc::Order::get()
	{
		return lpc_->order;
	}

	void Frame::Subframe::SubframeLpc::Order::set(UInt32 value)
	{
		lpc_->order = value;
	}

	UInt32 Frame::Subframe::SubframeLpc::QlpCoeffPrecision::get()
	{
		return lpc_->qlp_coeff_precision;
	}

	void Frame::Subframe::SubframeLpc::QlpCoeffPrecision::set(UInt32 value)
	{
		lpc_->qlp_coeff_precision = value;
	}

	Int32 Frame::Subframe::SubframeLpc::QuantizationLevel::get()
	{
		return lpc_->quantization_level;
	}

	void Frame::Subframe::SubframeLpc::QuantizationLevel::set(Int32 value)
	{
		lpc_->quantization_level = value;
	}

	Frame::Subframe::SubframeVerbatim::SubframeVerbatim(FLAC__Subframe_Verbatim* verbatim)
		: verbatim_(verbatim)
	{
	}

	Frame::Subframe::SubframeConstant^ Frame::Subframe::Constant::get()
	{
		return constant_;
	}

	Frame::Subframe::SubframeFixed^ Frame::Subframe::Fixed::get()
	{
		return fixed_;
	}

	Frame::Subframe::SubframeLpc^ Frame::Subframe::Lpc::get()
	{
		return lpc_;
	}

	Frame::Subframe::SubframeVerbatim^ Frame::Subframe::Verbatim::get()
	{
		return verbatim_;
	}

	// ---------------------------

	Frame::FrameFooter::FrameFooter(::FLAC__FrameFooter* frameFooter)
	{
		frameFooter_ = frameFooter;
	}

	UInt16 Frame::FrameFooter::Crc::get()
	{
		return frameFooter_->crc;
	}
}
