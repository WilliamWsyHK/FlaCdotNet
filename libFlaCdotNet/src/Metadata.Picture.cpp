// Metadata.Picture.cpp

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
#include "Metadata.Picture.h"
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace FlaCdotNet
{
	namespace Metadata
	{
		Picture::Picture()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Picture(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_PICTURE), false)
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			prototype_ = new FLAC::Metadata::Picture(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_PICTURE), false);
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

		Picture::Picture(::FLAC__StreamMetadata* object, bool copy)
			: Prototype()
		{
			prototype_ = new FLAC::Metadata::Picture(object, copy);
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

		FLAC::Metadata::Picture* Picture::picture_::get()
		{
			return dynamic_cast<FLAC::Metadata::Picture*>(prototype_);
		}

		FlaCdotNet::MetadataPictureType Picture::PictureType::get()
		{
			return GetPictureType();
		}

		void Picture::PictureType::set(FlaCdotNet::MetadataPictureType value)
		{
			SetPictureType(value);
		}

		String^ Picture::MimeType::get()
		{
			return GetMimeType();
		}

		void Picture::MimeType::set(String^ value)
		{
			SetMimeType(value);
		}

		String^ Picture::Description::get()
		{
			return GetDescription();
		}

		void Picture::Description::set(String^ value)
		{
			SetDescription(value);
		}

		UInt32 Picture::Width::get()
		{
			return GetWidth();
		}

		void Picture::Width::set(UInt32 value)
		{
			SetWidth(value);
		}

		UInt32 Picture::Height::get()
		{
			return GetHeight();
		}

		void Picture::Height::set(UInt32 value)
		{
			SetHeight(value);
		}

		UInt32 Picture::Depth::get()
		{
			return GetDepth();
		}

		void Picture::Depth::set(UInt32 value)
		{
			SetDepth(value);
		}

		UInt32 Picture::Colors::get()
		{
			return GetColors();
		}

		void Picture::Colors::set(UInt32 value)
		{
			SetColors(value);
		}

		array<Byte>^ Picture::Data::get()
		{
			return GetData();
		}

		void Picture::Data::set(array<Byte>^ value)
		{
			SetData(value);
		}

		UInt32 Picture::DataLength::get()
		{
			return GetDataLength();
		}

		Picture^ Picture::Assign(::FLAC__StreamMetadata* object, bool copy)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			picture_->assign(object, copy);
			return this;
		}

		Picture^ Picture::Assign(IntPtr object, bool copy)
		{
			return Assign(static_cast<::FLAC__StreamMetadata*>(object.ToPointer()), copy);
		}

		bool Picture::IsLegal([Out] String^% violation)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			const char* v;
			bool result = picture_->is_legal(&v);
			if (violation != nullptr)
			{
				violation = gcnew String(v);
			}
			return result;
		}

		FlaCdotNet::MetadataPictureType Picture::GetPictureType()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			return static_cast<FlaCdotNet::MetadataPictureType>(picture_->get_type());
		}

		String^ Picture::GetMimeType()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			return gcnew String(picture_->get_mime_type());
		}

		String^ Picture::GetDescription()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			const Byte* d = picture_->get_description();
			array<Byte>^ utf8Bytes = gcnew array<Byte>(sizeof(d));
			Marshal::Copy(IntPtr(const_cast<Byte*>(d)), utf8Bytes, 0, utf8Bytes->Length);
			return Encoding::UTF8->GetString(utf8Bytes)->Trim(L' ', L'\0');
		}

		UInt32 Picture::GetWidth()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			return picture_->get_width();
		}

		UInt32 Picture::GetHeight()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			return picture_->get_height();
		}

		UInt32 Picture::GetDepth()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			return picture_->get_depth();
		}

		UInt32 Picture::GetColors()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			return picture_->get_colors();
		}

		UInt32 Picture::GetDataLength()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return picture_->get_data_length();
		}

		array<Byte>^ Picture::GetData()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			array<Byte>^ data = gcnew array<Byte>(picture_->get_data_length());
			Marshal::Copy(IntPtr(const_cast<Byte*>(picture_->get_data())), data, 0, data->Length);
			return data;
		}

		void Picture::SetPictureType(FlaCdotNet::MetadataPictureType type)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			picture_->set_type(static_cast<::FLAC__StreamMetadata_Picture_Type>(type));
		}

		bool Picture::SetMimeType(String^ string)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			array<Byte>^ ansiBytes = Encoding::ASCII->GetBytes(string + "\0");
			pin_ptr<Byte> p = &ansiBytes[0];
			return picture_->set_mime_type(reinterpret_cast<char*>(p));
		}

		bool Picture::SetDescription(String^ string)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			array<Byte>^ utf8Bytes = Encoding::UTF8->GetBytes(string + "\0");
			pin_ptr<Byte> desc = &utf8Bytes[0];
			return picture_->set_description(desc);
		}

		void Picture::SetWidth(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			picture_->set_width(value);
		}

		void Picture::SetHeight(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			picture_->set_height(value);
		}

		void Picture::SetDepth(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			picture_->set_depth(value);
		}

		void Picture::SetColors(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif

			picture_->set_colors(value);
		}

		bool Picture::SetData(array<Byte>^ data)
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

			pin_ptr<Byte> d = &data[0];
			return picture_->set_data(d, data->Length);
		}
	}
}
