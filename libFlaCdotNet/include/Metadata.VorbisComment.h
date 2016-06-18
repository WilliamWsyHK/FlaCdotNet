// Metadata.VorbisComment.h

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

#pragma once
#include "stdafx.h"
#include "Metadata.Prototype.h"
using namespace System;
using namespace System::Diagnostics::Contracts;

namespace FlaCdotNet
{
	namespace Metadata
	{
		public ref class VorbisComment
			: public Prototype
		{
		protected:
			property FLAC::Metadata::VorbisComment* vorbisComment_
			{
				FLAC::Metadata::VorbisComment* get();
			}
		public protected:
			VorbisComment(::FLAC__StreamMetadata* object, bool copy);
			VorbisComment^ Assign(::FLAC__StreamMetadata* object, bool copy);
		public:
			ref class Entry
			{
			private:
				// Finalizer
				!Entry();

				bool disposedValue
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
					= false
#endif
#endif
					;
			internal:
				Entry(FLAC::Metadata::VorbisComment::Entry object);
			protected:
			public protected:
				FLAC::Metadata::VorbisComment::Entry* entry_;
			public:
				Entry();
				Entry(String^ field);
				Entry(String^ fieldName, String^ fieldValue);

				// Dispose
				~Entry();

				property String^ Field
				{
					String^ get();
					void set(String^ value);
				}

				property String^ FieldName
				{
					String^ get();
					void set(String^ value);
				}

				property String^ FieldValue
				{
					String^ get();
					void set(String^ value);
				}

				virtual bool IsValid();

				// Getters

				String^ GetField();
				String^ GetFieldName();
				String^ GetFieldValue();

				// Setters

				bool SetField(String^ field);
				bool SetFieldName(String^ fieldName);
				bool SetFieldValue(String^ fieldValue);
			};

			VorbisComment();

			property UInt32 NumComments
			{
				UInt32 get();
			}

			property String^ VendorString
			{
				String^ get();
				void set(String^ value);
			}

			property Entry^ default[UInt32]
			{
				Entry^ get(UInt32 index);
				void set(UInt32 index, Entry^ value);
			}

			VorbisComment^ Clone();
			VorbisComment^ Assign(IntPtr object, bool copy);

			// Getters

			UInt32 GetNumComments();
			String^ GetVendorString();
			Entry^ GetComment(UInt32 index);

			// Setters

			bool SetVendorString(String^ string);
			bool SetComment(UInt32 index, Entry^ entry);

			bool ResizeComments(UInt32 newNumComments);
			bool InsertComment(UInt32 index, Entry^ entry);
			bool AppendComment(Entry^ entry);
			bool ReplaceComment(Entry^ entry, bool all);
			bool DeleteComment(UInt32 index);

			Int32 FindEntryFrom(UInt32 offset, String^ fieldName);
			Int32 RemoveEntryMatching(String^ fieldName);
			Int32 RemoveEntriesMatching(String^ fieldName);

		protected:
			bool needUpdate
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
				= true
#endif
#endif
				;
			void updateComments();
			array<Entry^>^ comments_;
		};
	}
}
