// Encoder.EncoderFile.cpp

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

#include "Encoder.h"
using namespace System::Runtime::InteropServices;

namespace FlaCdotNet
{
	namespace Encoder
	{
		EncoderFile::EncoderFile(Encoder::Stream^ managedHandle, bool isFile)
			: FLAC::Encoder::File()
		{
			managedHandle_ = new gcroot<Encoder::Stream^>;
			*managedHandle_ = managedHandle;
			isFile_ = isFile;
		}

		EncoderFile::~EncoderFile()
		{
			*managedHandle_ = nullptr;
			delete managedHandle_;
		}

		::FLAC__StreamEncoderInitStatus EncoderFile::init()
		{
			return FLAC__stream_encoder_init_stream(encoder_, writeCallback_, seekCallback_, tellCallback_, metadataCallback_, managedHandle_);
		}

		::FLAC__StreamEncoderInitStatus EncoderFile::init_ogg()
		{
			return FLAC__stream_encoder_init_ogg_stream(encoder_, readCallback_, writeCallback_, seekCallback_, tellCallback_, metadataCallback_, managedHandle_);
		}

		::FLAC__StreamEncoderInitStatus EncoderFile::init(FILE* file)
		{
			if (!isFile_)
			{
				throw gcnew InvalidProgramException();
			}
			return FLAC__stream_encoder_init_FILE(encoder_, file, progressCallback_, managedHandle_);
		}

		::FLAC__StreamEncoderInitStatus EncoderFile::init_ogg(FILE* file)
		{
			if (!isFile_)
			{
				throw gcnew InvalidProgramException();
			}
			return FLAC__stream_encoder_init_ogg_FILE(encoder_, file, progressCallback_, managedHandle_);
		}

		::FLAC__StreamEncoderInitStatus EncoderFile::init(const char* fileName)
		{
			if (!isFile_)
			{
				throw gcnew InvalidProgramException();
			}
			return FLAC__stream_encoder_init_file(encoder_, fileName, progressCallback_, managedHandle_);
		}

		::FLAC__StreamEncoderInitStatus EncoderFile::init_ogg(const char* fileName)
		{
			if (!isFile_)
			{
				throw gcnew InvalidProgramException();
			}
			return FLAC__stream_encoder_init_ogg_file(encoder_, fileName, progressCallback_, managedHandle_);
		}

		::FLAC__StreamEncoderReadStatus EncoderFile::readCallback_(const ::FLAC__StreamEncoder* encoder, Byte buffer[], size_t* bytes, void* clientData)
		{
			gcroot<Encoder::Stream^> instance = *(reinterpret_cast<gcroot<Encoder::Stream^>*>(clientData));

			array<Byte>^ b = gcnew array<Byte>(*bytes);
			::FLAC__StreamEncoderReadStatus result = static_cast<::FLAC__StreamEncoderReadStatus>(instance->readCallback(b, *bytes));
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

		::FLAC__StreamEncoderWriteStatus EncoderFile::writeCallback_(const ::FLAC__StreamEncoder* encoder, const Byte buffer[], size_t bytes, UInt32 samples, UInt32 currentFrame, void* clientData)
		{
			gcroot<Encoder::Stream^> instance = *(reinterpret_cast<gcroot<Encoder::Stream^>*>(clientData));

			array<Byte>^ b = gcnew array<Byte>(bytes);
			for (Int64 i = 0L; i < b->LongLength; i++)
			{
#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif
				b[i] = buffer[i];
#ifdef _MSC_VER
#pragma warning (default: 4244)
#endif
			}
			::FLAC__StreamEncoderWriteStatus result = static_cast<::FLAC__StreamEncoderWriteStatus>(instance->writeCallback(b, bytes, samples, currentFrame));

			return result;
		}

		::FLAC__StreamEncoderSeekStatus EncoderFile::seekCallback_(const ::FLAC__StreamEncoder* encoder, UInt64 absoluteByteOffset, void* clientData)
		{
			gcroot<Encoder::Stream^> instance = *(reinterpret_cast<gcroot<Encoder::Stream^>*>(clientData));

			::FLAC__StreamEncoderSeekStatus result = static_cast<::FLAC__StreamEncoderSeekStatus>(instance->seekCallback(absoluteByteOffset));

			return result;
		}

		::FLAC__StreamEncoderTellStatus EncoderFile::tellCallback_(const ::FLAC__StreamEncoder* encoder, UInt64* absoluteByteOffset, void* clientData)
		{
			gcroot<Encoder::Stream^> instance = *(reinterpret_cast<gcroot<Encoder::Stream^>*>(clientData));

			::FLAC__StreamEncoderTellStatus result = static_cast<::FLAC__StreamEncoderTellStatus>(instance->tellCallback(*absoluteByteOffset));

			return result;
		}

		void EncoderFile::metadataCallback_(const ::FLAC__StreamEncoder* encoder, const ::FLAC__StreamMetadata* metadata, void* clientData)
		{
			gcroot<Encoder::Stream^> instance = *(reinterpret_cast<gcroot<Encoder::Stream^>*>(clientData));

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

		void EncoderFile::progressCallback_(const ::FLAC__StreamEncoder* encoder, UInt64 bytesWritten, UInt64 samplesWritten, UInt32 framesWritten, UInt32 totalFramesEstimate, void* clientData)
		{
			gcroot<Encoder::File^>* instance = reinterpret_cast<gcroot<Encoder::File^>*>(clientData);

			if (instance == nullptr)
			{
				throw gcnew InvalidProgramException();
			}

			(*instance)->progressCallback(bytesWritten, samplesWritten, framesWritten, totalFramesEstimate);
		}
	}
}
