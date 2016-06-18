// Metadata.SeekTable.cpp

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

#include "Metadata.SeekTable.h"

namespace FlaCdotNet
{
	namespace Metadata
	{
		SeekTable::SeekTable()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: SeekTable(::FLAC__metadata_object_new(::FLAC__MetadataType::FLAC__METADATA_TYPE_SEEKTABLE), false)
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			prototype_ = new FLAC::Metadata::SeekTable(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_SEEKTABLE), false);
			selfConstruct = true;
			needUpdate = true;
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

		SeekTable::SeekTable(::FLAC__StreamMetadata* object, bool copy)
			: Prototype()
		{
			prototype_ = new FLAC::Metadata::SeekTable(object, copy);
			selfConstruct = true;
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			needUpdate = true;
#endif
#endif
#ifdef CONTRACTS_FULL
			Contract::Ensures(prototype_ != nullptr);
#else
			if (!(prototype_ != nullptr))
			{
				throw gcnew OutOfMemoryException();
			}
#endif
		}

		FLAC::Metadata::SeekTable* SeekTable::seekTable_::get()
		{
			return dynamic_cast<FLAC::Metadata::SeekTable*>(prototype_);
		}

		UInt32 SeekTable::NumPoints::get()
		{
			return GetNumPoints();
		}

		SeekTable::SeekPoint^ SeekTable::default::get(UInt32 index)
		{
			return GetPoint(index);
		}

		void SeekTable::default::set(UInt32 index, SeekPoint^ value)
		{
			SetPoint(index, value);
		}

		SeekTable^ SeekTable::Clone()
		{
			return dynamic_cast<SeekTable^>(Prototype::Clone());
		}

		SeekTable^ SeekTable::Assign(::FLAC__StreamMetadata* object, bool copy)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			seekTable_->assign(object, copy);
			return this;
		}

		SeekTable^ SeekTable::Assign(IntPtr object, bool copy)
		{
			return Assign((::FLAC__StreamMetadata*)object.ToPointer(), copy);
		}

		void SeekTable::updatePoints()
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
			UInt32 numPoints = GetNumPoints();
			if (points_ == nullptr/* || static_cast<UInt32>(points_->LongLength) != numPoints*/)
			{
				points_ = gcnew array<SeekPoint^>(numPoints);
				// Always create new SeekPoint object instance.
				for (Int64 i = 0L; i < points_->LongLength; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					points_[i] = gcnew SeekPoint(seekTable_->get_point(i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
				return;
			}

			// resized to contain less points
			// so here we should dispose unnecessary points
			if (numPoints < points_->LongLength)
			{
				for (Int64 i = numPoints; i < points_->LongLength; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					SeekPoint^ point = points_[i];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
					if (point != nullptr)
					{
						delete point;
						if (point->IsValid())
						{
							throw gcnew InvalidOperationException();
						}
						point = nullptr;
					}
				}
				Array::Resize(points_, (Int32)numPoints);
			}
			// Update the existing pointers.
			for (Int64 i = 0L; i < points_->LongLength; i++)
			{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
				if (points_[i] != nullptr)
				{
					delete points_[i];
				}
				points_[i] = gcnew SeekPoint(seekTable_->get_point((UInt32)i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
			}
			// resized to contain more points
			// so here we should only add more
			if (points_->LongLength < numPoints)
			{
				Int64 oldLength = points_->LongLength;
				Array::Resize(points_, (Int32)numPoints);
				for (Int64 i = oldLength; i < numPoints; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					points_[i] = gcnew SeekPoint(seekTable_->get_point((UInt32)i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
			}
			needUpdate = false;
		}

		UInt32 SeekTable::GetNumPoints()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return seekTable_->get_num_points();
		}

		SeekTable::SeekPoint^ SeekTable::GetPoint(UInt32 index)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(index < GetNumPoints());
			Contract::Requires(IsValid());
#else
			if (!(index < GetNumPoints()))
			{
				throw gcnew ArgumentOutOfRangeException();
			}
			if (!IsValid())
			{
				throw gcnew InvalidOperationException("Invalid");
			}
#endif
			updatePoints();
			return points_[index];
		}

		bool SeekTable::IsLegal()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return seekTable_->is_legal();
		}

		bool SeekTable::ResizePoints(UInt32 newNumPoints)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = seekTable_->resize_points(newNumPoints);
			needUpdate = true;
			return result;
		}

		void SeekTable::SetPoint(UInt32 index, SeekPoint^ point)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(index < GetNumPoints());
			Contract::Requires(IsValid());
#else
			if (!(index < GetNumPoints()))
			{
				throw gcnew ArgumentOutOfRangeException();
			}
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			seekTable_->set_point(index, *(point->point_));
			needUpdate = true;
		}

		bool SeekTable::InsertPoint(UInt32 index, SeekPoint^ point)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(index <= GetNumPoints());
			Contract::Requires(IsValid());
#else
			if (!(index <= GetNumPoints()))
			{
				throw gcnew ArgumentOutOfRangeException();
			}
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = seekTable_->insert_point(index, *(point->point_));
			needUpdate = true;
			return result;
		}

		bool SeekTable::DeletePoint(UInt32 index)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(index < GetNumPoints());
			Contract::Requires(IsValid());
#else
			if (!(index < GetNumPoints()))
			{
				throw gcnew ArgumentOutOfRangeException();
			}
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = seekTable_->delete_point(index);
			needUpdate = true;
			return result;
		}

		bool SeekTable::TemplateAppendPlaceholders(UInt32 num)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
#ifdef _MSC_VER
#pragma warning (disable: 4800)
#endif
			bool result = seekTable_->template_append_placeholders(num);
#ifdef _MSC_VER
#pragma warning (default: 4800)
#endif
			needUpdate = true;
			return result;
		}

		bool SeekTable::TemplateAppendPoint(UInt64 sampleNumber)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
#ifdef _MSC_VER
#pragma warning (disable: 4800)
#endif
			bool result = seekTable_->template_append_point(sampleNumber);
#ifdef _MSC_VER
#pragma warning (default: 4800)
#endif
			needUpdate = true;
			return result;
		}

		bool SeekTable::TemplateAppendPoints(array<UInt64>^ sampleNumbers)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			pin_ptr<UInt64> p = &sampleNumbers[0];
			bool result = seekTable_->template_append_points(p, (UInt32)sampleNumbers->LongLength);
			needUpdate = true;
			return result;
		}

		bool SeekTable::TemplateAppendSpacedPoints(UInt32 num, UInt64 totalSamples)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = seekTable_->template_append_spaced_points(num, totalSamples);
			needUpdate = true;
			return result;
		}

		bool SeekTable::TemplateAppendSpacedPointsBySamples(UInt32 samples, UInt64 totalSamples)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = seekTable_->template_append_spaced_points_by_samples(samples, totalSamples);
			needUpdate = true;
			return result;
		}

		bool SeekTable::TemplateSort(bool compact)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = seekTable_->template_sort(compact);
			needUpdate = true;
			return result;
		}
	}
}
