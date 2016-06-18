// Metadata.CueSheet.cpp

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
using namespace System::Text;

namespace FlaCdotNet
{
	namespace Metadata
	{
		CueSheet::CueSheet()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: CueSheet(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_CUESHEET), false)
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			prototype_ = new FLAC::Metadata::CueSheet(::FLAC__metadata_object_new(::FLAC__METADATA_TYPE_CUESHEET), false);
			needUpdate = true;
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

		CueSheet::CueSheet(::FLAC__StreamMetadata* object, bool copy)
			: Prototype()
		{
			prototype_ = new FLAC::Metadata::CueSheet(object, copy);
			needUpdate = true;
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

		FLAC::Metadata::CueSheet* CueSheet::cueSheet_::get()
		{
			return dynamic_cast<FLAC::Metadata::CueSheet*>(prototype_);
		}

		String^ CueSheet::MediaCatalogNumber::get()
		{
			return GetMediaCatalogNumber();
		}

		void CueSheet::MediaCatalogNumber::set(String^ value)
		{
			SetMediaCatalogNumber(value);
		}

		UInt64 CueSheet::LeadIn::get()
		{
			return GetLeadIn();
		}

		void CueSheet::LeadIn::set(UInt64 value)
		{
			SetLeadIn(value);
		}

		bool CueSheet::IsCD::get()
		{
			return GetIsCD();
		}

		void CueSheet::IsCD::set(bool value)
		{
			SetIsCD(value);
		}

		UInt32 CueSheet::NumTracks::get()
		{
			return GetNumTracks();
		}

		CueSheet::Track^ CueSheet::default::get(UInt32 index)
		{
			return GetTrack(index);
		}

		void CueSheet::default::set(UInt32 index, Track^ value)
		{
			SetTrack(index, value);
		}

		CueSheet^ CueSheet::Assign(::FLAC__StreamMetadata* object, bool copy)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			cueSheet_->assign(object, copy);
			return this;
		}

		CueSheet^ CueSheet::Assign(IntPtr object, bool copy)
		{
			return Assign(static_cast<::FLAC__StreamMetadata*>(object.ToPointer()), copy);
		}

		void CueSheet::updateTracks()
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
			UInt32 numTracks = GetNumTracks();
			if (tracks_ == nullptr)
			{
				tracks_ = gcnew array<Track^>(numTracks);
				// Always create new Track object instance.
				for (Int64 i = 0; i < tracks_->LongLength; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					tracks_[i] = gcnew Track(cueSheet_->get_track((UInt32)i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
				return;
			}

			// resized to contain less tracks
			// so here we should dispose unnecessary tracks
			if (numTracks < tracks_->LongLength)
			{
				for (Int64 i = numTracks; i < tracks_->LongLength; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					Track^ track = tracks_[i];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
					if (track != nullptr)
					{
						delete track;
						if (track->IsValid())
						{
							throw gcnew InvalidProgramException();
						}
						track = nullptr;
					}
				}
				Array::Resize(tracks_, (Int32)numTracks);
			}
			// Update the existing pointers.
			for (Int64 i = 0; i < tracks_->LongLength; i++)
			{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
				if (tracks_[i] != nullptr)
				{
					delete tracks_[i];
				}
				tracks_[i] = gcnew Track(cueSheet_->get_track((UInt32)i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
			}
			// resized to contain more tracks
			// so here we should only add more
			if (tracks_->LongLength < numTracks)
			{
				Int64 oldLength = tracks_->LongLength;
				Array::Resize(tracks_, numTracks);
				for (Int64 i = oldLength; i < numTracks; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					tracks_[i] = gcnew Track(cueSheet_->get_track((UInt32)i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
			}
			needUpdate = false;
		}

		bool CueSheet::IsLegal(bool checkCddaSubset, [Out] String^% violation)
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
			bool result = cueSheet_->is_legal(checkCddaSubset, &v);
			if (violation != nullptr)
			{
				violation = gcnew String(v);
			}
			return result;
		}

		String^ CueSheet::GetMediaCatalogNumber()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return gcnew String(cueSheet_->get_media_catalog_number());
		}

		UInt64 CueSheet::GetLeadIn()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return cueSheet_->get_lead_in();
		}

		bool CueSheet::GetIsCD()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return cueSheet_->get_is_cd();
		}

		UInt32 CueSheet::GetNumTracks()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return cueSheet_->get_num_tracks();
		}

		CueSheet::Track^ CueSheet::GetTrack(UInt32 i)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			updateTracks();
			return tracks_[i];
		}

		void CueSheet::SetMediaCatalogNumber(String^ value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
			Contract::Requires(value->Length <= 128);
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
			if (!(value->Length <= 128))
			{
				throw gcnew ArgumentOutOfRangeException("value", "Length cannot be greater than 128");
			}
#endif
			array<Byte>^ utf8Bytes = Encoding::UTF8->GetBytes(value + "\0");
			pin_ptr<Byte> v = &utf8Bytes[0];
			cueSheet_->set_media_catalog_number(reinterpret_cast<char*>(v));
		}

		void CueSheet::SetLeadIn(UInt64 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			cueSheet_->set_lead_in(value);
		}

		void CueSheet::SetIsCD(bool value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			cueSheet_->set_is_cd(value);
		}

		void CueSheet::SetIndex(UInt32 trackNum, UInt32 indexNum, Index^ index)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			cueSheet_->set_index(trackNum, indexNum, *(index->index_));
			needUpdate = true;
		}

		bool CueSheet::SetTrack(UInt32 i, Track^ track)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = cueSheet_->set_track(i, *(track->track_));
			needUpdate = true;
			return result;
		}

		bool CueSheet::ResizeIndices(UInt32 trackNum, UInt32 newNumIndices)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = cueSheet_->resize_indices(trackNum, newNumIndices);
			needUpdate = true;
			return result;
		}

		bool CueSheet::InsertIndex(UInt32 trackNum, UInt32 indexNum, Index^ index)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = cueSheet_->insert_index(trackNum, indexNum, *(index->index_));
			needUpdate = true;
			return result;
		}

		bool CueSheet::InsertBlankIndex(UInt32 trackNum, UInt32 indexNum)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = cueSheet_->insert_blank_index(trackNum, indexNum);
			needUpdate = true;
			return result;
		}

		bool CueSheet::DeleteIndex(UInt32 trackNum, UInt32 indexNum)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = cueSheet_->delete_index(trackNum, indexNum);
			needUpdate = true;
			return result;
		}

		bool CueSheet::ResizeTracks(UInt32 newNumTracks)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = cueSheet_->resize_tracks(newNumTracks);
			needUpdate = true;
			return result;
		}

		bool CueSheet::InsertTrack(UInt32 i, Track^ track)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = cueSheet_->insert_track(i, *(track->track_));
			needUpdate = true;
			return result;
		}

		bool CueSheet::InsertBlankTrack(UInt32 i)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = cueSheet_->insert_blank_track(i);
			needUpdate = true;
			return result;
		}

		bool CueSheet::DeleteTrack(UInt32 i)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			bool result = cueSheet_->delete_track(i);
			needUpdate = true;
			return result;
		}

		UInt32 CueSheet::CalculateCDdbID()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			UInt32 result = cueSheet_->calculate_cddb_id();
			return result;
		}
	}
}
