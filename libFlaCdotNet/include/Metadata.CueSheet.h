// Metadata.CueSheet.h

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
using namespace System::Runtime::InteropServices;

namespace FlaCdotNet
{
	namespace Metadata
	{
		public ref class CueSheet
			: public Prototype
		{
		protected:
			property FLAC::Metadata::CueSheet* cueSheet_
			{
				FLAC::Metadata::CueSheet* get();
			}
		public protected:
			CueSheet(::FLAC__StreamMetadata* object, bool copy);
			CueSheet^ Assign(::FLAC__StreamMetadata* object, bool copy);
		public:
			ref class Index
			{
			private:
				// Finalizer
				!Index();

				bool disposedValue
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
					= false
#endif
#endif
					;
			internal:
				Index(::FLAC__StreamMetadata_CueSheet_Index index);
			protected:
				initonly bool selfConstruct
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
					= false
#endif
#endif
					;
			public protected:
				::FLAC__StreamMetadata_CueSheet_Index* index_;
			public:
				Index();

				// Dispose
				~Index();

				property UInt64 Offset
				{
					UInt64 get();
					void set(UInt64 value);
				}

				property Byte Number
				{
					Byte get();
					void set(Byte value);
				}

				bool IsValid();

				// Getters

				UInt64 GetOffset();
				Byte GetNumber();

				// Setters

				void SetOffset(UInt64 value);
				void SetNumber(Byte value);
			};

			ref class Track
			{
			private:
				// Finalizer
				!Track();

				bool disposedValue
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
					= false
#endif
#endif
					;
			internal:
				Track(FLAC::Metadata::CueSheet::Track track);
			protected:
				initonly bool selfConstruct
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
					= false
#endif
#endif
					;
				void updateIndices();
				array<Index^>^ indices_;
			public protected:
				FLAC::Metadata::CueSheet::Track* track_;
				bool needUpdate
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
					= true
#endif
#endif
					;
			public:
				Track();

				// Dispose
				~Track();

				virtual bool IsValid();

				property UInt64 Offset
				{
					UInt64 get();
					void set(UInt64 value);
				}

				property Byte Number
				{
					Byte get();
					void set(Byte value);
				}

				property String^ ISRC
				{
					String^ get();
					void set(String^ value);
				}

				property UInt32 TrackType
				{
					UInt32 get();
					void set(UInt32 value);
				}

				property bool PreEmphasis
				{
					bool get();
					void set(bool value);
				}

				property Byte NumIndices
				{
					Byte get();
				}

				property Index^ default[UInt32]
				{
					Index^ get(UInt32 index);
					void set(UInt32 index, Index^ value);
				}

					// Getters

				UInt64 GetOffset();
				Byte GetNumber();
				String^ GetISRC();
				UInt32 GetTrackType();
				bool GetPreEmphasis();
				Byte GetNumIndices();
				Index^ GetIndex(UInt32 i);

				// Setters

				void SetOffset(UInt64 value);
				void SetNumber(Byte value);
				void SetISRC(String^ value);
				void SetTrackType(UInt32 value);
				void SetPreEmphasis(bool value);
				void SetIndex(UInt32 i, Index^ index);
			};

			CueSheet();

			property String^ MediaCatalogNumber
			{
				String^ get();
				void set(String^ value);
			}

			property UInt64 LeadIn
			{
				UInt64 get();
				void set(UInt64 value);
			}

			property bool IsCD
			{
				bool get();
				void set(bool value);
			}

			property UInt32 NumTracks
			{
				UInt32 get();
			}

			property Track^ default[UInt32]
			{
				Track^ get(UInt32 index);
				void set(UInt32 index, Track^ value);
			}

			CueSheet^ Assign(IntPtr object, bool copy);

			// Getters

			bool IsLegal(bool checkCddaSubset, [Out] String^% violation);
			String^ GetMediaCatalogNumber();
			UInt64 GetLeadIn();
			bool GetIsCD();
			UInt32 GetNumTracks();
			Track^ GetTrack(UInt32 i);

			// Setters

			void SetMediaCatalogNumber(String^ value);
			void SetLeadIn(UInt64 value);
			void SetIsCD(bool value);
			void SetIndex(UInt32 trackNum, UInt32 indexNum, Index^ index);
			bool SetTrack(UInt32 i, Track^ track);

			bool ResizeIndices(UInt32 trackNum, UInt32 newNumIndices);
			bool InsertIndex(UInt32 trackNum, UInt32 indexNum, Index^ index);
			bool InsertBlankIndex(UInt32 trackNum, UInt32 indexNum);
			bool DeleteIndex(UInt32 trackNum, UInt32 indexNum);

			bool ResizeTracks(UInt32 newNumTracks);
			bool InsertTrack(UInt32 i, Track^ track);
			bool InsertBlankTrack(UInt32 i);
			bool DeleteTrack(UInt32 i);

			UInt32 CalculateCDdbID();
		protected:
			bool needUpdate
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
				= true
#endif
#endif
				;
			void updateTracks();
			array<Track^>^ tracks_;
		};
	}
}
