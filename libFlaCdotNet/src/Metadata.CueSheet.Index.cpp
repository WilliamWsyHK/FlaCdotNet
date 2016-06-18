// Metadata.CueSheet.Index.cpp

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

#include "Metadata.CueSheet.h"

namespace FlaCdotNet
{
	namespace Metadata
	{
		CueSheet::Index::Index()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Index(::FLAC__StreamMetadata_CueSheet_Index())
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			index_ = new ::FLAC__StreamMetadata_CueSheet_Index();
#endif
#endif
			selfConstruct = true;
		}

		CueSheet::Index::Index(::FLAC__StreamMetadata_CueSheet_Index index)
			: index_(new ::FLAC__StreamMetadata_CueSheet_Index(index))
		{
		}

		CueSheet::Index::~Index()
		{
			if (!disposedValue)
			{
				this->!Index();

				disposedValue = true;
			}
		}

		CueSheet::Index::!Index()
		{
			if (selfConstruct)
			{
				if (index_ != nullptr)
				{
					delete index_;

					index_ = nullptr;
				}
			}
		}

		UInt64 CueSheet::Index::Offset::get()
		{
			return GetOffset();
		}

		void CueSheet::Index::Offset::set(UInt64 value)
		{
			SetOffset(value);
		}

		Byte CueSheet::Index::Number::get()
		{
			return GetNumber();
		}

		void CueSheet::Index::Number::set(Byte value)
		{
			SetNumber(value);
		}

		bool CueSheet::Index::IsValid()
		{
			if (disposedValue)
			{
				throw gcnew ObjectDisposedException(this->GetType()->FullName);
			}
			return (index_ != nullptr);
		}

		UInt64 CueSheet::Index::GetOffset()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return index_->offset;
		}

		Byte CueSheet::Index::GetNumber()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return index_->number;
		}

		void CueSheet::Index::SetOffset(UInt64 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			index_->offset = value;
		}

		void CueSheet::Index::SetNumber(Byte value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			index_->number = value;
		}
	}
}
