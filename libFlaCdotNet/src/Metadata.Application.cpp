// Metadata.Application.cpp

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

#include "Metadata.Application.h"
using namespace System::Runtime::InteropServices;

namespace FlaCdotNet
{
	namespace Metadata
	{
		Application::Application()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Application(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_APPLICATION), false)
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			prototype_ = new FLAC::Metadata::Application(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_APPLICATION), false);
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

		Application::Application(::FLAC__StreamMetadata* object, bool copy)
			: Prototype()
		{
			prototype_ = new FLAC::Metadata::Application(object, copy);
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

		FLAC::Metadata::Application* Application::application_::get()
		{
			return dynamic_cast<FLAC::Metadata::Application*>(prototype_);
		}

		array<Byte>^ Application::ID::get()
		{
			return GetID();
		}

		void Application::ID::set(array<Byte>^ value)
		{
			SetID(value);
		}

		array<Byte>^ Application::Data::get()
		{
			return GetData();
		}

		void Application::Data::set(array<Byte>^ value)
		{
			SetData(value);
		}

		Application^ Application::Clone()
		{
			return safe_cast<Application^>(Prototype::Clone());
		}

		Application^ Application::Assign(::FLAC__StreamMetadata* object, bool copy)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			application_->assign(object, copy);
			return this;
		}

		Application^ Application::Assign(IntPtr object, bool copy)
		{
			return Assign(static_cast<::FLAC__StreamMetadata*>(object.ToPointer()), copy);
		}

		array<Byte>^ Application::GetID()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			array<Byte>^ id = gcnew array<Byte>(4);
			Marshal::Copy(IntPtr(const_cast<Byte*>(application_->get_id())), id, 0, id->Length);
			return id;
		}

		array<Byte>^ Application::GetData()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			const Byte* d = application_->get_data();
			array<Byte>^ data = gcnew array<Byte>(sizeof(d));
			Marshal::Copy(IntPtr(const_cast<Byte*>(d)), data, 0, data->Length);
			return data;
		}

		void Application::SetID(array<Byte>^ value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(value->Length <= 4);
			Contract::Requires(IsValid());
#else
			if (!(value->Length <= 4))
			{
				throw gcnew ArgumentException("value cannot have Length > 4");
			}
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			pin_ptr<Byte> id = &value[0];
			application_->set_id(id);
		}

		bool Application::SetData(array<Byte>^ data)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(data != nullptr);
			Contract::Requires(IsValid());
#else
			if (!(data != nullptr))
			{
				throw gcnew ArgumentNullException("data");
			}
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return SetData(data, true);
		}

		bool Application::SetData(array<Byte>^ data, bool copy)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			UInt32 length = data == nullptr ? 0u : (UInt32)data->LongLength;
			if (!((data != nullptr) || (data == nullptr && length == 0 && !copy)))
			{
				throw gcnew ArgumentException("If data array is null, then copy cannot be true");
			}

			if (data != nullptr)
			{
				pin_ptr<Byte> d = &data[0];
				return application_->set_data(d, length, copy);
			}
			else
			{
				return application_->set_data(nullptr, length, copy);
			}
		}
	}
}
