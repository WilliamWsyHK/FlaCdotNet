// Metadata.VorbisComment.cpp

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

#include "Metadata.VorbisComment.h"
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace FlaCdotNet
{
	namespace Metadata
	{
		VorbisComment::VorbisComment()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: VorbisComment(::FLAC__metadata_object_new(::FLAC__MetadataType::FLAC__METADATA_TYPE_VORBIS_COMMENT), false)
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			prototype_ = new FLAC::Metadata::VorbisComment(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_VORBIS_COMMENT), false);
			needUpdate = true;
			selfConstruct = true;
#ifdef CONTRACTS_FULL
			Contract::Ensures(prototype_ != nullptr);
#else
			if (!(prototype_ != nullptr))
			{
				throw gcnew OutOfMemoryException();
			}
			SetVendorString(FlaCdotNet::Constants::VendorString);
#endif
#endif
#endif
		}

		VorbisComment::VorbisComment(::FLAC__StreamMetadata* object, bool copy)
			: Prototype()
		{
			prototype_ = new FLAC::Metadata::VorbisComment(object, copy);
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			needUpdate = true;
#endif
#endif
			selfConstruct = true;
#ifdef CONTRACTS_FULL
			Contract::Ensures(prototype_ != nullptr);
#else
			if (!(prototype_ != nullptr))
			{
				throw gcnew OutOfMemoryException();
			}
#endif
			SetVendorString(FlaCdotNet::Constants::VendorString);
		}

		FLAC::Metadata::VorbisComment* VorbisComment::vorbisComment_::get()
		{
			return dynamic_cast<FLAC::Metadata::VorbisComment*>(prototype_);
		}

		UInt32 VorbisComment::NumComments::get()
		{
			return GetNumComments();
		}

		String^ VorbisComment::VendorString::get()
		{
			return GetVendorString();
		}

		void VorbisComment::VendorString::set(String^ value)
		{
			SetVendorString(value);
		}

		VorbisComment::Entry^ VorbisComment::default::get(UInt32 index)
		{
			return GetComment(index);
		}

		void VorbisComment::default::set(UInt32 index, Entry^ value)
		{
			SetComment(index, value);
		}

		VorbisComment^ VorbisComment::Clone()
		{
			return dynamic_cast<VorbisComment^>(Prototype::Clone());
		}

		VorbisComment^ VorbisComment::Assign(::FLAC__StreamMetadata* object, bool copy)
		{
			vorbisComment_->assign(object, copy);
			return this;
		}

		VorbisComment^ VorbisComment::Assign(IntPtr object, bool copy)
		{
			return Assign(static_cast<::FLAC__StreamMetadata*>(object.ToPointer()), copy);
		}

		void VorbisComment::updateComments()
		{
			if (!needUpdate)
			{
				return;
			}
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			UInt32 numComments = GetNumComments();
			if (comments_ == nullptr/* || static_cast<UInt32>(comments_->LongLength) != numComments*/)
			{
				comments_ = gcnew array<Entry^>(numComments);
				// Always create new Entry object instance.
				for (Int64 i = 0; i < comments_->LongLength; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					comments_[i] = gcnew Entry(vorbisComment_->get_comment((UInt32)i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
				return;
			}

			// resized to contain less comments
			// so here we should dispose unnecessary comments
			if (numComments < comments_->LongLength)
			{
				for (Int64 i = numComments; i < comments_->LongLength; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					Entry^ entry = comments_[i];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
					if (entry != nullptr)
					{
						delete entry;
						if (entry->IsValid())
						{
							throw gcnew InvalidProgramException();
						}
						entry = nullptr;
					}
				}
				Array::Resize(comments_, (Int32)numComments);
			}
			// Update the existing pointers.
			for (Int64 i = 0; i < comments_->LongLength; i++)
			{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
				if (comments_[i] != nullptr)
				{
					delete comments_[i];
				}
				comments_[i] = gcnew Entry(vorbisComment_->get_comment((UInt32)i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
			}
			// resized to contain more comments
			// so here we should only add more
			if (comments_->LongLength < numComments)
			{
				Int64 oldLength = comments_->LongLength;
				Array::Resize(comments_, numComments);
				for (Int64 i = oldLength; i < numComments; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					comments_[i] = gcnew Entry(vorbisComment_->get_comment((UInt32)i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
			}
			needUpdate = false;
		}

		UInt32 VorbisComment::GetNumComments()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return vorbisComment_->get_num_comments();
		}

		String^ VorbisComment::GetVendorString()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			String^ result;
			{
				String^ forcedAnsiString = Marshal::PtrToStringAnsi(IntPtr(const_cast<Byte*>(vorbisComment_->get_vendor_string())));
				array<Byte>^ utf8Bytes = Encoding::UTF8->GetBytes(forcedAnsiString);
				result = Encoding::UTF8->GetString(utf8Bytes);
			}
			return result->Trim(' ', '\0');
		}

		VorbisComment::Entry^ VorbisComment::GetComment(UInt32 index)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			updateComments();
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
			return comments_[index];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
		}

		bool VorbisComment::SetVendorString(String^ string)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(string != nullptr);
			Contract::Requires(IsValid());
#else
			if (!(string != nullptr))
			{
				throw gcnew ArgumentNullException("string");
			}
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			array<Byte>^ utf8Bytes = Encoding::UTF8->GetBytes(string + "\0");
			pin_ptr<Byte> p = &utf8Bytes[0];
			return vorbisComment_->set_vendor_string(p);
		}

		bool VorbisComment::SetComment(UInt32 index, Entry^ entry)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = vorbisComment_->set_comment(index, *(entry->entry_));
			needUpdate = true;
			return result;
		}

		bool VorbisComment::ResizeComments(UInt32 newNumComments)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = vorbisComment_->resize_comments(newNumComments);
			needUpdate = true;
			return result;
		}

		bool VorbisComment::InsertComment(UInt32 index, Entry^ entry)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = vorbisComment_->insert_comment(index, *(entry->entry_));
			needUpdate = true;
			return result;
		}

		bool VorbisComment::AppendComment(Entry^ entry)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = vorbisComment_->append_comment(*(entry->entry_));
			needUpdate = true;
			return result;
		}

		bool VorbisComment::ReplaceComment(Entry^ entry, bool all)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = vorbisComment_->replace_comment(*(entry->entry_), all);
			needUpdate = true;
			return result;
		}

		bool VorbisComment::DeleteComment(UInt32 index)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = vorbisComment_->delete_comment(index);
			needUpdate = true;
			return result;
		}

		Int32 VorbisComment::FindEntryFrom(UInt32 offset, String^ fieldName)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			char* name = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());
			Int32 result = vorbisComment_->find_entry_from(offset, name);
			Marshal::FreeHGlobal(IntPtr(name));
			return result;
		}

		Int32 VorbisComment::RemoveEntryMatching(String^ fieldName)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			char* name = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());
			Int32 result = vorbisComment_->remove_entry_matching(name);
			Marshal::FreeHGlobal(IntPtr(name));
			return result;
		}

		Int32 VorbisComment::RemoveEntriesMatching(String^ fieldName)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			char* name = static_cast<char*>(Marshal::StringToHGlobalAnsi(fieldName).ToPointer());
			Int32 result = vorbisComment_->remove_entries_matching(name);
			Marshal::FreeHGlobal(IntPtr(name));
			return result;
		}
	}
}
