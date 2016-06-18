// Metadata.Unknown.cpp

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

#include "Metadata.Unknown.h"
using namespace System::Runtime::InteropServices;

namespace FlaCdotNet
{
	namespace Metadata
	{
		Unknown::Unknown()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Unknown(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_APPLICATION), false)
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			prototype_ = new FLAC::Metadata::Unknown(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_APPLICATION), false);
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

		Unknown::Unknown(::FLAC__StreamMetadata* object, bool copy)
			: Prototype()
		{
			prototype_ = new FLAC::Metadata::Unknown(object, copy);
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

		FLAC::Metadata::Unknown* Unknown::unknown_::get()
		{
			return dynamic_cast<FLAC::Metadata::Unknown*>(prototype_);
		}

		array<Byte>^ Unknown::Data::get()
		{
			return GetData();
		}

		void Unknown::Data::set(array<Byte>^ value)
		{
			SetData(value);
		}

		Unknown^ Unknown::Assign(::FLAC__StreamMetadata* object, bool copy)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			unknown_->assign(object, copy);
			return this;
		}

		Unknown^ Unknown::Assign(IntPtr object, bool copy)
		{
			return Assign(static_cast<::FLAC__StreamMetadata*>(object.ToPointer()), copy);
		}

		array<Byte>^ Unknown::GetData()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			const Byte* d = unknown_->get_data();

			// Hack: use the id to minus the 4-byte offset
			Int32 idLength = strnlen((char*)unknown_->operator const FLAC__StreamMetadata *()->data.application.id, 4);
			array<Byte>^ data = gcnew array<Byte>(idLength + sizeof(d));

			Marshal::Copy(IntPtr((void*)unknown_->operator const FLAC__StreamMetadata *()->data.application.id), data, 0, idLength);
			// complete hack

			Marshal::Copy(IntPtr(const_cast<Byte*>(d)), data, idLength, data->Length - idLength);
			return data;
		}

		bool Unknown::SetData(array<Byte>^ data)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(data != nullptr);
			Contract::Requires(IsValid());
#else
			if (!(data != nullptr))
			{
				throw gcnew ArgumentNullException();
			}
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return SetData(data, true);
		}

		bool Unknown::SetData(array<Byte>^ data, bool copy)
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
				// Hack: use the id to minus the 4-byte offset
				UInt32 copiedLength = Math::Min(4, data->Length);
				Marshal::Copy(data, 0, IntPtr((void*)unknown_->operator const FLAC__StreamMetadata *()->data.application.id), copiedLength);
				if (data->Length <= 4)
				{
					return true;
				}
				// complete hack

				return unknown_->set_data(&d[copiedLength], length - copiedLength, copy);
			}
			else
			{
				// Hack: use the id to minus the 4-byte offset
				{
					array<Byte>^ dummy = gcnew array<Byte>(4);
					Marshal::Copy(dummy, 0, IntPtr((void*)unknown_->operator const FLAC__StreamMetadata *()->data.application.id), 4);
				}
				// complete hack

				return unknown_->set_data(nullptr, length, copy);
			}
		}
	}
}
