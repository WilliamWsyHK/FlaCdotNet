// Format.EntropyCodingMethod.cpp

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
	EntropyCodingMethod::EntropyCodingMethod(FLAC__EntropyCodingMethod* entropyCodingMethod)
		: entropyCodingMethod_(entropyCodingMethod)
	{
		partitionedRice_ = gcnew EntropyCodingMethodPartitionedRice(&(entropyCodingMethod_->data.partitioned_rice));
	}

	EntropyCodingMethodType EntropyCodingMethod::EntropyCodingMethodType::get()
	{
		return static_cast<FlaCdotNet::EntropyCodingMethodType>(entropyCodingMethod_->type);
	}

	void EntropyCodingMethod::EntropyCodingMethodType::set(FlaCdotNet::EntropyCodingMethodType value)
	{
		entropyCodingMethod_->type = static_cast<FLAC__EntropyCodingMethodType>(value);
	}

	EntropyCodingMethod::EntropyCodingMethodPartitionedRice^ EntropyCodingMethod::PartitionedRice::get()
	{
		return partitionedRice_;
	}

	EntropyCodingMethod::EntropyCodingMethodPartitionedRice::EntropyCodingMethodPartitionedRice(FLAC__EntropyCodingMethod_PartitionedRice* partitionedRice)
		: partitionedRice_(partitionedRice)
	{
	}

	UInt32 EntropyCodingMethod::EntropyCodingMethodPartitionedRice::Order::get()
	{
		return partitionedRice_->order;
	}

	void EntropyCodingMethod::EntropyCodingMethodPartitionedRice::Order::set(UInt32 value)
	{
		partitionedRice_->order = value;
	}

	array<EntropyCodingMethod::EntropyCodingMethodPartitionedRice::PartitionedRiceContents^>^ EntropyCodingMethod::EntropyCodingMethodPartitionedRice::Contents::get()
	{
		// TODO: implement this
		throw gcnew NotImplementedException();
	}

	EntropyCodingMethod::EntropyCodingMethodPartitionedRice::PartitionedRiceContents::PartitionedRiceContents(FLAC__EntropyCodingMethod_PartitionedRiceContents* contents)
		: contents_(contents)
	{
	}
}
