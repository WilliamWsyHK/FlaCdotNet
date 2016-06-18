// Metadata.CueSheet.Track.cpp

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
		CueSheet::Track::Track()
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
			: Track(FLAC::Metadata::CueSheet::Track())
#endif
#endif
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			: track_(new FLAC::Metadata::CueSheet::Track())
#endif
#endif
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			needUpdate = true;
#endif
#endif
			selfConstruct = true;
		}

		CueSheet::Track::Track(FLAC::Metadata::CueSheet::Track track)
			: track_(new FLAC::Metadata::CueSheet::Track(track))
		{
#ifdef _MSC_VER
#if (_MSC_VER <= 1700)
			needUpdate = true;
#endif
#endif
		}

		CueSheet::Track::~Track()
		{
			if (!disposedValue)
			{
				this->!Track();

				disposedValue = true;
			}
		}

		CueSheet::Track::!Track()
		{
			if (selfConstruct)
			{
				if (track_ != nullptr)
				{
					delete track_;

					track_ = nullptr;
				}
			}
		}

		bool CueSheet::Track::IsValid()
		{
			if (disposedValue)
			{
				throw gcnew ObjectDisposedException(this->GetType()->FullName);
			}
			return (track_ != nullptr) && track_->is_valid();
		}

		UInt64 CueSheet::Track::Offset::get()
		{
			return GetOffset();
		}

		void CueSheet::Track::Offset::set(UInt64 value)
		{
			SetOffset(value);
		}

		Byte CueSheet::Track::Number::get()
		{
			return GetNumber();
		}

		void CueSheet::Track::Number::set(Byte value)
		{
			SetNumber(value);
		}

		String^ CueSheet::Track::ISRC::get()
		{
			return GetISRC();
		}

		void CueSheet::Track::ISRC::set(String^ value)
		{
			SetISRC(value);
		}

		UInt32 CueSheet::Track::TrackType::get()
		{
			return GetTrackType();
		}

		void CueSheet::Track::TrackType::set(UInt32 value)
		{
			SetTrackType(value);
		}

		bool CueSheet::Track::PreEmphasis::get()
		{
			return GetPreEmphasis();
		}

		void CueSheet::Track::PreEmphasis::set(bool value)
		{
			SetPreEmphasis(value);
		}

		Byte CueSheet::Track::NumIndices::get()
		{
			return GetNumIndices();
		}

		CueSheet::Index^ CueSheet::Track::default::get(UInt32 index)
		{
			return GetIndex(index);
		}

		void CueSheet::Track::default::set(UInt32 index, Index^ value)
		{
			SetIndex(index, value);
		}

		void CueSheet::Track::updateIndices()
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
			Byte numIndices = GetNumIndices();
			if (indices_ == nullptr)
			{
				indices_ = gcnew array<Index^>(numIndices);
				// Always create new Index object instance.
				for (Int32 i = 0; i < indices_->Length; i++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					indices_[i] = gcnew Index(track_->get_index((UInt32)i));
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
				return;
			}

			// resized to contain less indices
			// so here we should dispose unnecessary indices
			if (numIndices < indices_->Length)
			{
				for (Int32 i = 0; i < indices_->Length; i++)
				{
					Index^ index = indices_[i];
					if (index == nullptr)
					{
						delete index;
						if (index->IsValid())
						{
							throw gcnew InvalidProgramException();
						}
						index = nullptr;
					}
				}
				Array::Resize(indices_, numIndices);
			}
			// Update the existing pointers.
			for (Int32 i = 0; i < indices_->Length; i++)
			{
				if (indices_[i] != nullptr)
				{
					delete indices_[i];
				}
				indices_[i] = gcnew Index(track_->get_index((UInt32)i));
			}
			// resized to contain more indices
			// so here we should only add more
			if (indices_->Length < numIndices)
			{
				Int32 oldLength = indices_->Length;
				Array::Resize(indices_, numIndices);
				for (Int32 i = oldLength; i < numIndices; i++)
				{
					indices_[i] = gcnew Index(track_->get_index((UInt32)i));
				}
			}
			needUpdate = false;
		}

		UInt64 CueSheet::Track::GetOffset()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return track_->get_offset();
		}

		Byte CueSheet::Track::GetNumber()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return track_->get_number();
		}

		String^ CueSheet::Track::GetISRC()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return gcnew String(track_->get_isrc());
		}

		UInt32 CueSheet::Track::GetTrackType()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return track_->get_type();
		}

		bool CueSheet::Track::GetPreEmphasis()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return track_->get_pre_emphasis();
		}

		Byte CueSheet::Track::GetNumIndices()
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			return track_->get_num_indices();
		}

		CueSheet::Index^ CueSheet::Track::GetIndex(UInt32 i)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			updateIndices();
			return indices_[i];
		}

		void CueSheet::Track::SetOffset(UInt64 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			track_->set_offset(value);
		}

		void CueSheet::Track::SetNumber(Byte value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			track_->set_number(value);
		}

		void CueSheet::Track::SetISRC(String^ value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
			Contract::Requires(value->Length <= 12);
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
			if (!(value->Length <= 12))
			{
				throw gcnew ArgumentOutOfRangeException("value", "Length cannot be greater than 12");
			}
#endif
			array<Byte>^ v = Encoding::ASCII->GetBytes(value + "\0");
			pin_ptr<Byte> p = &v[0];
			track_->set_isrc(reinterpret_cast<char*>(p));
		}

		void CueSheet::Track::SetTrackType(UInt32 value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			track_->set_type(value);
		}

		void CueSheet::Track::SetPreEmphasis(bool value)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			track_->set_pre_emphasis(value);
		}

		void CueSheet::Track::SetIndex(UInt32 i, Index^ index)
		{
#ifdef CONTRACTS_FULL
			Contract::Requires(IsValid());
#else
			if (!IsValid())
			{
				throw gcnew InvalidOperationException();
			}
#endif
			track_->set_index(i, *(index->index_));
			needUpdate = true;
		}
	}
}
