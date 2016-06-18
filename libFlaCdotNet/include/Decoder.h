// Decoder.h

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
#include <vcclr.h>
#include "Stdafx.h"
#include "Format.h"
#include "Metadata.h"
using namespace System;

namespace FlaCdotNet
{
	namespace Decoder
	{
		public enum class State
		{
			SearchForMetadata     = ::FLAC__STREAM_DECODER_SEARCH_FOR_METADATA,
			ReadMetadata          = ::FLAC__STREAM_DECODER_READ_METADATA,
			SearchForFrameSync    = ::FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC,
			ReadFrame             = ::FLAC__STREAM_DECODER_READ_FRAME,
			EndOfStream           = ::FLAC__STREAM_DECODER_END_OF_STREAM,
			OggError              = ::FLAC__STREAM_DECODER_OGG_ERROR,
			SeekError             = ::FLAC__STREAM_DECODER_SEEK_ERROR,
			Aborted               = ::FLAC__STREAM_DECODER_ABORTED,
			MemoryAllocationError = ::FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR,
			Uninitialized         = ::FLAC__STREAM_DECODER_UNINITIALIZED
		};

		public enum class InitStatus
		{
			Ok                    = ::FLAC__STREAM_DECODER_INIT_STATUS_OK,
			UnsupportedContainer  = ::FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER,
			InvalidCallbacks      = ::FLAC__STREAM_DECODER_INIT_STATUS_INVALID_CALLBACKS,
			MemoryAllocationError = ::FLAC__STREAM_DECODER_INIT_STATUS_MEMORY_ALLOCATION_ERROR,
			ErrorOpeningFile      = ::FLAC__STREAM_DECODER_INIT_STATUS_ERROR_OPENING_FILE,
			AlreadyInitialized    = ::FLAC__STREAM_DECODER_INIT_STATUS_ALREADY_INITIALIZED
		};

		public enum class ReadStatus
		{
			Continue    = ::FLAC__STREAM_DECODER_READ_STATUS_CONTINUE,
			EndOfStream = ::FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM,
			Abort       = ::FLAC__STREAM_DECODER_READ_STATUS_ABORT
		};

		public enum class SeekStatus
		{
			Ok          = ::FLAC__STREAM_DECODER_SEEK_STATUS_OK,
			Error       = ::FLAC__STREAM_DECODER_SEEK_STATUS_ERROR,
			Unsupported = ::FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED
		};

		public enum class TellStatus
		{
			Ok          = ::FLAC__STREAM_DECODER_TELL_STATUS_OK,
			Error       = ::FLAC__STREAM_DECODER_TELL_STATUS_ERROR,
			Unsupported = ::FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED
		};

		public enum class LengthStatus
		{
			Ok          = ::FLAC__STREAM_DECODER_LENGTH_STATUS_OK,
			Error       = ::FLAC__STREAM_DECODER_LENGTH_STATUS_ERROR,
			Unsupported = ::FLAC__STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED
		};

		public enum class WriteStatus
		{
			Continue = ::FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE,
			Abort    = ::FLAC__STREAM_DECODER_WRITE_STATUS_ABORT
		};

		public enum class ErrorStatus
		{
			LostSync          = ::FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC,
			BadHeader         = ::FLAC__STREAM_DECODER_ERROR_STATUS_BAD_HEADER,
			FrameCrcMismatch  = ::FLAC__STREAM_DECODER_ERROR_STATUS_FRAME_CRC_MISMATCH,
			UnparseableStream = ::FLAC__STREAM_DECODER_ERROR_STATUS_UNPARSEABLE_STREAM
		};

		ref class Stream;

		private class DecoderFile
			: public FLAC::Decoder::File
		{
		public:
			bool isFile_
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
				= false
#endif
#endif
				;
			gcroot<Decoder::Stream^>* managedHandle_;

			DecoderFile(Decoder::Stream^ managedHandle, bool isFile);

			~DecoderFile();

			virtual ::FLAC__StreamDecoderInitStatus init() override;
			virtual ::FLAC__StreamDecoderInitStatus init_ogg() override;

			virtual ::FLAC__StreamDecoderInitStatus init(FILE* file) override;
			virtual ::FLAC__StreamDecoderInitStatus init_ogg(FILE* file) override;

			virtual ::FLAC__StreamDecoderInitStatus init(const char* fileName) override;
			virtual ::FLAC__StreamDecoderInitStatus init_ogg(const char* fileName) override;

			// Dummy Write Callback, only to satisfy the abstract (pure virtual) method condition.
			virtual ::FLAC__StreamDecoderWriteStatus write_callback(const ::FLAC__Frame* frame, const Int32* const buffer[]) override;

			// Dummy Error Callback, only to satisfy the abstract (pure virtual) method condition.
			virtual void error_callback(::FLAC__StreamDecoderErrorStatus status) override;

			static ::FLAC__StreamDecoderReadStatus   readCallback_(const ::FLAC__StreamDecoder* decoder, Byte buffer[], size_t* bytes, void* clientData);
			static ::FLAC__StreamDecoderSeekStatus   seekCallback_(const ::FLAC__StreamDecoder* decoder, UInt64 absoluteByteOffset, void* clientData);
			static ::FLAC__StreamDecoderTellStatus   tellCallback_(const ::FLAC__StreamDecoder* decoder, UInt64* absoluteByteOffset, void* clientData);
			static ::FLAC__StreamDecoderLengthStatus lengthCallback_(const ::FLAC__StreamDecoder* decoder, UInt64* streamLength, void* clientData);
			static FLAC__bool                        eofCallback_(const ::FLAC__StreamDecoder* decoder, void* clientData);
			static ::FLAC__StreamDecoderWriteStatus  writeCallback_(const ::FLAC__StreamDecoder* decoder, const ::FLAC__Frame* frame, const Int32* const buffer[], void* clientData);
			static void                              metadataCallback_(const ::FLAC__StreamDecoder* decoder, const ::FLAC__StreamMetadata* metadata, void* clientData);
			static void                              errorCallback_(const ::FLAC__StreamDecoder* decoder, ::FLAC__StreamDecoderErrorStatus status, void* clientData);
		};

		public ref class Stream abstract
		{
		private:
			// Finalizer
			!Stream();

			bool disposedValue
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
				= false
#endif
#endif
				;
		protected:
			DecoderFile* decoder_;
		public protected:
			virtual ReadStatus   readCallback(array<Byte>^ buffer, size_t% bytes) abstract;
			virtual SeekStatus   seekCallback(UInt64 absoluteByteOffset);
			virtual TellStatus   tellCallback(UInt64% absoluteByteOffset);
			virtual LengthStatus lengthCallback(UInt64% streamLength);
			virtual bool         eofCallback();
			virtual WriteStatus  writeCallback(Frame^ frame, array<Int32, 2>^ buffer) abstract;
			virtual void         metadataCallback(Metadata::Prototype^ metadata);
			virtual void         errorCallback(ErrorStatus status) abstract;
		public:
			ref class State
			{
			private:
				// Finalizer
				!State();

				bool disposedValue
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
					= false
#endif
#endif
					;
			internal:
				State(FLAC::Decoder::Stream::State state);
			protected:
				FLAC::Decoder::Stream::State* state_;
			public:
				// Dispose
				~State();

				bool IsValid();

				static operator FlaCdotNet::Decoder::State(State^ state);

				virtual String^ ToString() override;
				String^ Resolve(Stream^ stream);
			};

			Stream();

			// Dispose
			~Stream();

			virtual bool IsValid();

			virtual InitStatus Init();
			virtual InitStatus InitOgg();

			virtual bool Finish();
			virtual bool Flush();
			virtual bool Reset();

			virtual bool ProcessSingle();
			virtual bool ProcessUntilEndOfMetadata();
			virtual bool ProcessUntilEndOfStream();
			virtual bool SkipSingleFrame();
			virtual bool SeekAbsolute(UInt64 sample);

			// Getters

			State^ GetState();
			virtual bool GetMD5Checking();
			virtual UInt64 GetTotalSamples();
			virtual UInt32 GetChannels();
			virtual FlaCdotNet::ChannelAssignment GetChannelAssignment();
			virtual UInt32 GetBitsPerSample();
			virtual UInt32 GetSampleRate();
			virtual UInt32 GetBlockSize();
			virtual bool GetDecodePosition(UInt64% position);

			// Setters

			virtual bool SetOggSerialNumber(Int32 value);
			virtual bool SetMD5Checking(bool value);
			virtual bool SetMetadataRespond(FlaCdotNet::MetadataType type);
			virtual bool SetMetadataRespondApplication(array<Byte>^ id);
			virtual bool SetMetadataRespondAll();
			virtual bool SetMetadataIgnore(FlaCdotNet::MetadataType type);
			virtual bool SetMetadataIgnoreApplication(array<Byte>^ id);
			virtual bool SetMetadataIgnoreAll();
		};

		public ref class File abstract
			: public Stream
		{
		private:
			// Finalize
			!File();

			bool disposedValue
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
				= false
#endif
#endif
				;
		public protected:
			virtual ReadStatus   readCallback(array<Byte>^ buffer, size_t% bytes) override sealed;
		public:
			File();

			// Dispose
			~File();

			virtual InitStatus Init(String^ fileName);
			virtual InitStatus InitOgg(String^ fileName);
		};
	}
}
