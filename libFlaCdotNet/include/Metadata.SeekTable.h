// Metadata.SeekTable.h

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
		public ref class SeekTable
			: public Prototype
		{
		protected:
			property FLAC::Metadata::SeekTable* seekTable_
			{
				FLAC::Metadata::SeekTable* get();
			}
		public protected:
			SeekTable(::FLAC__StreamMetadata* object, bool copy);
			SeekTable^ Assign(::FLAC__StreamMetadata* object, bool copy);
		public:
			ref class SeekPoint
			{
			private:
				// Finalizer
				!SeekPoint();

				bool disposedValue
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
					= false
#endif
#endif
					;
			internal:
			protected:
				initonly bool selfConstruct
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
					= false
#endif
#endif
					;
			public protected:
				::FLAC__StreamMetadata_SeekPoint* point_;
				SeekPoint(::FLAC__StreamMetadata_SeekPoint object);
				SeekPoint(IntPtr object);
			public:
				SeekPoint();

				// Dispose
				~SeekPoint();

				property UInt64 SampleNumber
				{
					UInt64 get();
					void set(UInt64 value);
				}

				property UInt64 StreamOffset
				{
					UInt64 get();
					void set(UInt64 value);
				}

				property UInt32 FrameSamples
				{
					UInt32 get();
					void set(UInt32 value);
				}

				bool IsValid();

				UInt64 GetSampleNumber();
				UInt64 GetStreamOffset();
				UInt32 GetFrameSamples();

				void SetSampleNumber(UInt64 value);
				void SetStreamOffset(UInt64 value);
				void SetFrameSamples(UInt32 value);
			};

			SeekTable();

			property UInt32 NumPoints
			{
				UInt32 get();
			}

			property SeekPoint^ default[UInt32]
			{
				SeekPoint^ get(UInt32 index);
				void set(UInt32 index, SeekPoint^ value);
			}

			SeekTable^ Clone();
			SeekTable^ Assign(IntPtr object, bool copy);

			// Getters

			UInt32 SeekTable::GetNumPoints();
			SeekPoint^ GetPoint(UInt32 index);
			bool IsLegal();

			// Setters

			bool ResizePoints(UInt32 newNumPoints);
			void SetPoint(UInt32 index, SeekPoint^ point);
			bool InsertPoint(UInt32 index, SeekPoint^ point);
			bool DeletePoint(UInt32 index);

			bool TemplateAppendPlaceholders(UInt32 num);
			bool TemplateAppendPoint(UInt64 sampleNumber);
			bool TemplateAppendPoints(array<UInt64>^ sampleNumbers);
			bool TemplateAppendSpacedPoints(UInt32 num, UInt64 totalSamples);
			bool TemplateAppendSpacedPointsBySamples(UInt32 samples, UInt64 totalSamples);
			bool TemplateSort(bool compact);

		protected:
			bool needUpdate
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
				= true
#endif
#endif
				;
			void updatePoints();
			array<SeekPoint^>^ points_;
		};
	}
}
