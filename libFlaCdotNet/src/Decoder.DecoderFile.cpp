// Decoder.DecoderFile.cpp

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

#include "Decoder.h"
#include "Metadata.h"
using namespace System::Runtime::InteropServices;

namespace FlaCdotNet
{
	namespace Decoder
	{
		DecoderFile::DecoderFile(Decoder::Stream^ managedHandle, bool isFile)
			: FLAC::Decoder::File()
		{
			managedHandle_ = new gcroot<Decoder::Stream^>;
			*managedHandle_ = managedHandle;
			isFile_ = isFile;
		}

		DecoderFile::~DecoderFile()
		{
			*managedHandle_ = nullptr;
			delete managedHandle_;
		}

		::FLAC__StreamDecoderInitStatus DecoderFile::init()
		{
			return ::FLAC__stream_decoder_init_stream(decoder_, readCallback_, seekCallback_, tellCallback_, lengthCallback_, eofCallback_, writeCallback_, metadataCallback_, errorCallback_, managedHandle_);
		}

		::FLAC__StreamDecoderInitStatus DecoderFile::init_ogg()
		{
			return ::FLAC__stream_decoder_init_ogg_stream(decoder_, readCallback_, seekCallback_, tellCallback_, lengthCallback_, eofCallback_, writeCallback_, metadataCallback_, errorCallback_, managedHandle_);
		}

		::FLAC__StreamDecoderInitStatus DecoderFile::init(FILE* file)
		{
			if (!isFile_)
			{
				throw gcnew InvalidProgramException();
			}
			return ::FLAC__stream_decoder_init_FILE(decoder_, file, writeCallback_, metadataCallback_, errorCallback_, managedHandle_);
		}

		::FLAC__StreamDecoderInitStatus DecoderFile::init_ogg(FILE* file)
		{
			if (!isFile_)
			{
				throw gcnew InvalidProgramException();
			}
			return ::FLAC__stream_decoder_init_ogg_FILE(decoder_, file, writeCallback_, metadataCallback_, errorCallback_, managedHandle_);
		}

		::FLAC__StreamDecoderInitStatus DecoderFile::init(const char* fileName)
		{
			if (!isFile_)
			{
				throw gcnew InvalidProgramException();
			}
			return ::FLAC__stream_decoder_init_file(decoder_, fileName, writeCallback_, metadataCallback_, errorCallback_, managedHandle_);
		}

		::FLAC__StreamDecoderInitStatus DecoderFile::init_ogg(const char* fileName)
		{
			if (!isFile_)
			{
				throw gcnew InvalidProgramException();
			}
			return ::FLAC__stream_decoder_init_ogg_file(decoder_, fileName, writeCallback_, metadataCallback_, errorCallback_, managedHandle_);
		}

		// Dummy
		::FLAC__StreamDecoderWriteStatus DecoderFile::write_callback(const ::FLAC__Frame* frame, const Int32* const buffer[])
		{
			return ::FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
		}

		// Dummy
		void DecoderFile::error_callback(::FLAC__StreamDecoderErrorStatus status)
		{
		}

		::FLAC__StreamDecoderReadStatus DecoderFile::readCallback_(const ::FLAC__StreamDecoder* decoder, Byte buffer[], size_t* bytes, void* clientData)
		{
			gcroot<Decoder::Stream^> instance = *(reinterpret_cast<gcroot<Decoder::Stream^>*>(clientData));

			gcroot<array<Byte>^> b = gcnew array<Byte>(*bytes);
			::FLAC__StreamDecoderReadStatus result = static_cast<::FLAC__StreamDecoderReadStatus>(instance->readCallback(b, *bytes));
			/*for (size_t i = 0L; i < *bytes; i++)
			{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
				buffer[i] = b[i];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
			}*/
			Marshal::Copy(b, 0, IntPtr(buffer), *bytes);

			return result;
		}

		::FLAC__StreamDecoderSeekStatus DecoderFile::seekCallback_(const ::FLAC__StreamDecoder* decoder, UInt64 absoluteByteOffset, void* clientData)
		{
			gcroot<Decoder::Stream^> instance = *(reinterpret_cast<gcroot<Decoder::Stream^>*>(clientData));

			::FLAC__StreamDecoderSeekStatus result = static_cast<::FLAC__StreamDecoderSeekStatus>(instance->seekCallback(absoluteByteOffset));
			
			return result;
		}

		::FLAC__StreamDecoderTellStatus DecoderFile::tellCallback_(const ::FLAC__StreamDecoder* decoder, UInt64* absoluteByteOffset, void* clientData)
		{
			gcroot<Decoder::Stream^> instance = *(reinterpret_cast<gcroot<Decoder::Stream^>*>(clientData));

			::FLAC__StreamDecoderTellStatus result = static_cast<::FLAC__StreamDecoderTellStatus>(instance->tellCallback(*absoluteByteOffset));
			
			return result;
		}

		::FLAC__StreamDecoderLengthStatus DecoderFile::lengthCallback_(const ::FLAC__StreamDecoder* decoder, UInt64* streamLength, void* clientData)
		{
			gcroot<Decoder::Stream^> instance = *(reinterpret_cast<gcroot<Decoder::Stream^>*>(clientData));

			::FLAC__StreamDecoderLengthStatus result = static_cast<::FLAC__StreamDecoderLengthStatus>(instance->lengthCallback(*streamLength));
			
			return result;
		}

		FLAC__bool DecoderFile::eofCallback_(const ::FLAC__StreamDecoder* decoder, void* clientData)
		{
			gcroot<Decoder::Stream^> instance = *(reinterpret_cast<gcroot<Decoder::Stream^>*>(clientData));

			FLAC__bool result = instance->eofCallback();
			
			return result;
		}

		::FLAC__StreamDecoderWriteStatus DecoderFile::writeCallback_(const ::FLAC__StreamDecoder* decoder, const ::FLAC__Frame* frame, const Int32* const buffer[], void* clientData)
		{
			gcroot<Decoder::Stream^> instance = *(reinterpret_cast<gcroot<Decoder::Stream^>*>(clientData));

			gcroot<Frame^> f = gcnew Frame(frame);
			gcroot<array<Int32, 2>^> b = gcnew array<Int32, 2>(f->Header->Channels, f->Header->BlockSize);
			for (Int64 i = 0L; i < b->GetLongLength(0); i++)
			{
				for (Int64 j = 0L; j < b->GetLongLength(1); j++)
				{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
					b[i, j] = buffer[i][j];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
				}
			}
			::FLAC__StreamDecoderWriteStatus result = static_cast<::FLAC__StreamDecoderWriteStatus>(instance->writeCallback(f, b));
			
			return result;
		}

		void DecoderFile::metadataCallback_(const ::FLAC__StreamDecoder* decoder, const ::FLAC__StreamMetadata* metadata, void* clientData)
		{
			gcroot<Decoder::Stream^> instance = *(reinterpret_cast<gcroot<Decoder::Stream^>*>(clientData));

			::FLAC__StreamMetadata* meta = const_cast<::FLAC__StreamMetadata*>(metadata);
			gcroot<Metadata::Prototype^> m;
			switch (metadata->type)
			{
			case ::FLAC__METADATA_TYPE_STREAMINFO:
				m = gcnew Metadata::StreamInfo(meta, true);
				break;
			case ::FLAC__METADATA_TYPE_PADDING:
				m = gcnew Metadata::Padding(meta, true);
				break;
			case ::FLAC__METADATA_TYPE_APPLICATION:
				m = gcnew Metadata::Application(meta, true);
				break;
			case ::FLAC__METADATA_TYPE_SEEKTABLE:
				m = gcnew Metadata::SeekTable(meta, true);
				break;
			case ::FLAC__METADATA_TYPE_VORBIS_COMMENT:
				m = gcnew Metadata::VorbisComment(meta, true);
				break;
			case ::FLAC__METADATA_TYPE_CUESHEET:
				m = gcnew Metadata::CueSheet(meta, true);
				break;
			case ::FLAC__METADATA_TYPE_PICTURE:
				m = gcnew Metadata::Picture(meta, true);
				break;
			case ::FLAC__METADATA_TYPE_UNDEFINED:
				m = gcnew Metadata::Unknown(meta, true);
				break;
			}
			instance->metadataCallback(m);
		}

		void DecoderFile::errorCallback_(const ::FLAC__StreamDecoder* decoder, ::FLAC__StreamDecoderErrorStatus status, void* clientData)
		{
			gcroot<Decoder::Stream^> instance = *(reinterpret_cast<gcroot<Decoder::Stream^>*>(clientData));

			instance->errorCallback(static_cast<ErrorStatus>(status));
		}
	}
}
