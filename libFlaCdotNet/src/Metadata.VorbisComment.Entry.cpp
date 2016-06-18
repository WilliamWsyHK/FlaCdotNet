// Metadata.VorbisComment.Entry.cpp

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
#include <cstring>
#include "Metadata.VorbisComment.h"
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace FlaCdotNet
{
	namespace Metadata
	{
		VorbisComment::Entry::Entry()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Entry(FLAC::Metadata::VorbisComment::Entry())
#endif
#endif
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			: entry_(new FLAC::Metadata::VorbisComment::Entry())
#endif
#endif
		{
			if (entry_ == nullptr)
			{
				throw gcnew OutOfMemoryException();
			}
		}

		VorbisComment::Entry::Entry(FLAC::Metadata::VorbisComment::Entry object)
			: entry_(new FLAC::Metadata::VorbisComment::Entry(object))
		{
		}

		VorbisComment::Entry::Entry(String^ field)
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Entry()
#endif
#endif
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			: entry_(new FLAC::Metadata::VorbisComment::Entry())
#endif
#endif
		{
			array<Byte>^ f = Encoding::UTF8->GetBytes(field + "\0");
			pin_ptr<Byte> fPtr = &f[0];
			entry_->set_field(reinterpret_cast<const char*>(fPtr));
		}

		VorbisComment::Entry::Entry(String^ fieldName, String^ fieldValue)
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Entry()
#endif
#endif
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			: entry_(new FLAC::Metadata::VorbisComment::Entry())
#endif
#endif
		{
			array<Byte>^ name = Encoding::ASCII->GetBytes(fieldName + "\0");
			array<Byte>^ value = Encoding::UTF8->GetBytes(fieldValue + "\0");
			pin_ptr<Byte> nPtr = &name[0];
			pin_ptr<Byte> vPtr = &value[0];
			entry_->set_field_name(reinterpret_cast<const char*>(nPtr));
			entry_->set_field_value(reinterpret_cast<const char*>(vPtr));
		}

		VorbisComment::Entry::~Entry()
		{
			if (!disposedValue)
			{
				this->!Entry();

				disposedValue = true;
			}
		}

		VorbisComment::Entry::!Entry()
		{
			if (entry_ != nullptr)
			{
				delete entry_;
			}
		}

		String^ VorbisComment::Entry::Field::get()
		{
			return GetField();
		}

		void VorbisComment::Entry::Field::set(String^ value)
		{
			SetField(value);
		}

		String^ VorbisComment::Entry::FieldName::get()
		{
			return GetFieldName();
		}

		void VorbisComment::Entry::FieldName::set(String^ value)
		{
			SetFieldName(value);
		}

		String^ VorbisComment::Entry::FieldValue::get()
		{
			return GetFieldValue();
		}

		void VorbisComment::Entry::FieldValue::set(String^ value)
		{
			SetFieldValue(value);
		}

		bool VorbisComment::Entry::IsValid()
		{
			if (disposedValue)
			{
				throw gcnew ObjectDisposedException(this->GetType()->FullName);
			}
			return (entry_ != nullptr) && entry_->is_valid();
		}

		String^ VorbisComment::Entry::GetField()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			array<Byte>^ f = gcnew array<Byte>(entry_->get_field_length());
			Marshal::Copy(IntPtr(const_cast<char*>(entry_->get_field())), f, 0, f->Length);
			return Encoding::UTF8->GetString(f)->Trim(' ', '\0');
		}

		String^ VorbisComment::Entry::GetFieldName()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return GetField()->Split(L'=')[0];
		}

		String^ VorbisComment::Entry::GetFieldValue()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return GetField()->Split(L'=')[1];
		}

		bool VorbisComment::Entry::SetField(String^ field)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(field != nullptr);
#else
			if (!(field != nullptr))
			{
				throw gcnew ArgumentNullException("field");
			}
#endif
			array<Byte>^ f = Encoding::UTF8->GetBytes(field + "\0");
			pin_ptr<Byte> fPtr = &f[0];
			return entry_->set_field(reinterpret_cast<char*>(fPtr));
		}

		bool VorbisComment::Entry::SetFieldName(String^ fieldName)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(fieldName != nullptr);
#else
			if (!(fieldName != nullptr))
			{
				throw gcnew ArgumentNullException("fieldName");
			}
#endif
			array<Byte>^ n = Encoding::ASCII->GetBytes(fieldName + "\0");
			pin_ptr<Byte> nPtr = &n[0];
			return entry_->set_field_name(reinterpret_cast<char*>(nPtr));
		}

		bool VorbisComment::Entry::SetFieldValue(String^ fieldValue)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(fieldValue != nullptr);
#else
			if (!(fieldValue != nullptr))
			{
				throw gcnew ArgumentNullException("fieldValue");
			}
#endif
			array<Byte>^ v = Encoding::UTF8->GetBytes(fieldValue + "\0");
			pin_ptr<Byte> vPtr = &v[0];
			return entry_->set_field_value(reinterpret_cast<char*>(vPtr), v->Length);
		}
	}
}
