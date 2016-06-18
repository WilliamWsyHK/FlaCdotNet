// Metadata.Prototype.cpp

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

#include "stdafx.h"

#include "Metadata.Prototype.h"

namespace FlaCdotNet
{
	namespace Metadata
	{
		Prototype::Prototype()
			: prototype_(nullptr)
		{
		}

		Prototype::Prototype(Prototype^ object)
			: prototype_(FLAC::Metadata::clone(object->prototype_)),
			selfConstruct(true)
		{
		}

		Prototype::~Prototype()
		{
			if (!disposedValue)
			{
				this->!Prototype();
				disposedValue = true;
			}
		}

		Prototype::!Prototype()
		{
			if (!selfConstruct)
			{
				return;
			}
			if (prototype_ != nullptr)
			{
				delete prototype_;
				prototype_ = nullptr;
			}
		}

		bool Prototype::IsLast::get()
		{
			return GetIsLast();
		}

		void Prototype::IsLast::set(bool value)
		{
			SetIsLast(value);
		}

		MetadataType Prototype::MetadataType::get()
		{
			return GetMetadataType();
		}

		UInt32 Prototype::Length::get()
		{
			return GetLength();
		}

		// IEquatable<> Support
		bool Prototype::Equals(Prototype^ other)
		{
			return (((Object^)other) != nullptr) && (*prototype_ == *(other->prototype_));
		}

		bool Prototype::operator==(Prototype^ right)
		{
			return ReferenceEquals(this, right) && !(((Object^)this == nullptr) || ((Object^)right == nullptr)) && this->Equals(right);
		}

		bool Prototype::operator!=(Prototype^ right)
		{
			return !operator==(right);
		}

		bool Prototype::Equals(Object^ obj)
		{
			return (obj != nullptr) && (safe_cast<Prototype^>(obj) != nullptr) && (Equals(safe_cast<Prototype^>(obj)));
		}

		bool Prototype::IsValid()
		{
			if (disposedValue)
			{
				throw gcnew ObjectDisposedException(this->GetType()->FullName);
			}
			return (prototype_ != nullptr) && prototype_->is_valid();
		}

		Prototype^ Prototype::Clone()
		{
			return gcnew Prototype(this);
		}

		bool Prototype::GetIsLast()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return prototype_->get_is_last();
		}

		FlaCdotNet::MetadataType Prototype::GetMetadataType()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return static_cast<FlaCdotNet::MetadataType>(prototype_->get_type());
		}

		UInt32 Prototype::GetLength()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return prototype_->get_length();
		}

		void Prototype::SetIsLast(bool value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			prototype_->set_is_last(value);
		}
	}
}
