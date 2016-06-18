// Metadata.Padding.cpp

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

#include "Metadata.Padding.h"

namespace FlaCdotNet
{
	namespace Metadata
	{
		Padding::Padding()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Padding(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_PADDING), false)
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			prototype_ = new FLAC::Metadata::Padding(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_PADDING), false);
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

		Padding::Padding(::FLAC__StreamMetadata* object, bool copy)
			: Prototype()
		{
			prototype_ = new FLAC::Metadata::Padding(object, copy);
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

		Padding::Padding(UInt32 length)
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Padding()
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			prototype_ = new FLAC::Metadata::Padding(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_PADDING), false);
			selfConstruct = true;
#endif
#endif
			SetLength(length);
		}

		FLAC::Metadata::Padding* Padding::padding_::get()
		{
			return dynamic_cast<FLAC::Metadata::Padding*>(prototype_);
		}

		UInt32 Padding::Length::get()
		{
			return GetLength();
		}

		void Padding::Length::set(UInt32 value)
		{
			SetLength(value);
		}

		Padding^ Padding::Clone()
		{
			return dynamic_cast<Padding^>(Prototype::Clone());
		}

		Padding^ Padding::Assign(::FLAC__StreamMetadata* object, bool copy)
		{
			padding_->assign(object, copy);
			return this;
		}

		Padding^ Padding::Assign(IntPtr object, bool copy)
		{
			return Assign(static_cast<::FLAC__StreamMetadata*>(object.ToPointer()), copy);
		}

		UInt32 Padding::GetLength()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return Prototype::GetLength();
		}

		void Padding::SetLength(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			padding_->set_length(value);
		}
	}
}
