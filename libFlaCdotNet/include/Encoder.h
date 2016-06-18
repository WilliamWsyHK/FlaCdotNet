// Encoder.h

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
#include "Decoder.h"
#include "Metadata.h"
using namespace System;
using namespace System::Runtime::InteropServices;

namespace FlaCdotNet
{
	namespace Encoder
	{
		public enum class State
		{
			Ok                        = ::FLAC__STREAM_ENCODER_OK,
			Uninitialized             = ::FLAC__STREAM_ENCODER_UNINITIALIZED,
			OggError                  = ::FLAC__STREAM_ENCODER_OGG_ERROR,
			VerifyDecoderError        = ::FLAC__STREAM_ENCODER_VERIFY_DECODER_ERROR,
			VerifyMismatchInAudioData = ::FLAC__STREAM_ENCODER_VERIFY_MISMATCH_IN_AUDIO_DATA,
			ClientError               = ::FLAC__STREAM_ENCODER_CLIENT_ERROR,
			IoError                   = ::FLAC__STREAM_ENCODER_IO_ERROR,
			FramingError              = ::FLAC__STREAM_ENCODER_FRAMING_ERROR,
			MemoryAllocationError     = ::FLAC__STREAM_ENCODER_MEMORY_ALLOCATION_ERROR
		};

		public enum class InitStatus
		{
			Ok                           = ::FLAC__STREAM_ENCODER_INIT_STATUS_OK,
			EncoderError                 = ::FLAC__STREAM_ENCODER_INIT_STATUS_ENCODER_ERROR,
			UnsupportedContainer         = ::FLAC__STREAM_ENCODER_INIT_STATUS_UNSUPPORTED_CONTAINER,
			InvalidCallbacks             = ::FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_CALLBACKS,
			InvalidNumberOfChannels      = ::FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_NUMBER_OF_CHANNELS,
			InvalidBitsPerSample         = ::FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_BITS_PER_SAMPLE,
			InvalidSampleRate            = ::FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_SAMPLE_RATE,
			InvalidBlockSize             = ::FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_BLOCK_SIZE,
			InvalidMaxLpcOrder           = ::FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_MAX_LPC_ORDER,
			InvalidQlpCoeffPrecision     = ::FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_QLP_COEFF_PRECISION,
			BlockSizeTooSmallForLpcOrder = ::FLAC__STREAM_ENCODER_INIT_STATUS_BLOCK_SIZE_TOO_SMALL_FOR_LPC_ORDER,
			NotStreamable                = ::FLAC__STREAM_ENCODER_INIT_STATUS_NOT_STREAMABLE,
			InvalidMetadata              = ::FLAC__STREAM_ENCODER_INIT_STATUS_INVALID_METADATA,
			AlreadyInitialized           = ::FLAC__STREAM_ENCODER_INIT_STATUS_ALREADY_INITIALIZED
		};

		public enum class ReadStatus
		{
			Continue    = ::FLAC__STREAM_ENCODER_READ_STATUS_CONTINUE,
			EndOfStream = ::FLAC__STREAM_ENCODER_READ_STATUS_END_OF_STREAM,
			Abort       = ::FLAC__STREAM_ENCODER_READ_STATUS_ABORT,
			Unsupported = ::FLAC__STREAM_ENCODER_READ_STATUS_UNSUPPORTED
		};

		public enum class WriteStatus
		{
			Ok         = ::FLAC__STREAM_ENCODER_WRITE_STATUS_OK,
			FatalError = FLAC__STREAM_ENCODER_WRITE_STATUS_FATAL_ERROR
		};

		public enum class SeekStatus
		{
			Ok          = ::FLAC__STREAM_ENCODER_SEEK_STATUS_OK,
			Error       = FLAC__STREAM_ENCODER_SEEK_STATUS_ERROR,
			Unsupported = FLAC__STREAM_ENCODER_SEEK_STATUS_UNSUPPORTED
		};

		public enum class TellStatus
		{
			Ok          = ::FLAC__STREAM_ENCODER_TELL_STATUS_OK,
			Error       = FLAC__STREAM_ENCODER_TELL_STATUS_ERROR,
			Unsupported = FLAC__STREAM_ENCODER_TELL_STATUS_UNSUPPORTED
		};

		ref class Stream;

		private class EncoderFile
			: public FLAC::Encoder::File
		{
		public:
			bool isFile_
#ifdef _MSC_VER
#if (_MSC_VER >= 1800)
				= false
#endif
#endif
				;
			gcroot<Encoder::Stream^>* managedHandle_;

			EncoderFile(Encoder::Stream^ managedHandle, bool isFile);

			~EncoderFile();

			virtual ::FLAC__StreamEncoderInitStatus init() override;
			virtual ::FLAC__StreamEncoderInitStatus init_ogg() override;

			virtual ::FLAC__StreamEncoderInitStatus init(FILE* file) override;
			virtual ::FLAC__StreamEncoderInitStatus init_ogg(FILE* file) override;

			virtual ::FLAC__StreamEncoderInitStatus init(const char* fileName) override;
			virtual ::FLAC__StreamEncoderInitStatus init_ogg(const char* fileName) override;

			static ::FLAC__StreamEncoderReadStatus  readCallback_(const ::FLAC__StreamEncoder* encoder, Byte buffer[], size_t* bytes, void* clientData);
			static ::FLAC__StreamEncoderWriteStatus writeCallback_(const ::FLAC__StreamEncoder* encoder, const Byte buffer[], size_t bytes, UInt32 samples, UInt32 currentFrame, void* clientData);
			static ::FLAC__StreamEncoderSeekStatus  seekCallback_(const ::FLAC__StreamEncoder* encoder, UInt64 absoluteByteOffset, void* clientData);
			static ::FLAC__StreamEncoderTellStatus  tellCallback_(const ::FLAC__StreamEncoder* encoder, UInt64* absoluteByteOffset, void* clientData);
			static void                             metadataCallback_(const ::FLAC__StreamEncoder* encoder, const ::FLAC__StreamMetadata* metadata, void* clientData);
			static void                             progressCallback_(const ::FLAC__StreamEncoder* encoder, UInt64 bytesWritten, UInt64 samplesWritten, UInt32 framesWritten, UInt32 totalFramesEstimate, void* clientData);
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
			EncoderFile* encoder_;
			// For SetMetadata(), free when Finalize() being called
			FLAC::Metadata::Prototype** metadata_;
			UInt64 numMetadata_;
		public protected:
			virtual ReadStatus  readCallback(array<Byte>^ buffer, size_t% bytes);
			virtual WriteStatus writeCallback(array<Byte>^ buffer, size_t bytes, UInt32 samples, UInt32 currentFrame) abstract;
			virtual SeekStatus  seekCallback(UInt64 absoluteByteOffset);
			virtual TellStatus  tellCallback(UInt64% absoluteByteOffset);
			virtual void        metadataCallback(Metadata::Prototype^ metadata);
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
				State(FLAC::Encoder::Stream::State state);
			protected:
				FLAC::Encoder::Stream::State* state_;
			public:
				// Dispose
				~State();

				bool IsValid();

				static operator FlaCdotNet::Encoder::State(State^ state);

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

			virtual bool Process(array<Int32, 2>^ buffer, UInt32 samples);
			virtual bool ProcessInterleaved(array<Int32>^ buffer, UInt32 samples);

			// Getters

			State^  GetState();
			virtual Decoder::Stream::State^ GetVerifyDecoderState();
			virtual void GetVerifyDecoderErrorStats([Out] UInt64% absoluteSample, [Out] UInt32% frameNumber, [Out] UInt32% channel, [Out] UInt32% sample, [Out] Int32% expected, [Out] Int32% got);
			virtual bool GetVerify();
			virtual bool GetStreamableSubset();
			virtual bool GetDoMidSideStereo();
			virtual bool GetLooseMidSideStereo();

			virtual UInt32 GetChannels();
			virtual UInt32 GetBitsPerSample();
			virtual UInt32 GetSampleRate();
			virtual UInt32 GetBlockSize();
			virtual UInt32 GetMaxLpcOrder();
			virtual UInt32 GetQlpCoeffPrecision();

			virtual bool GetDoQlpCoeffPrecSearch();
			virtual bool GetDoEscapeCoding();
			virtual bool GetDoExhaustiveModelSearch();

			virtual UInt32 GetMinResidualPartitionOrder();
			virtual UInt32 GetMaxResidualPartitionOrder();
			virtual UInt32 GetRiceParameterSearchDist();

			virtual UInt64 GetTotalSamplesEstimate();

			// Setters

			virtual bool SetOggSerialNumber(Int32 value);
			virtual bool SetVerify(bool value);
			virtual bool SetStreamableSubset(bool value);
			virtual bool SetChannels(UInt32 value);
			virtual bool SetBitsPerSample(UInt32 value);
			virtual bool SetSampleRate(UInt32 value);
			virtual bool SetCompressionLevel(UInt32 value);
			virtual bool SetBlockSize(UInt32 value);
			virtual bool SetDoMidSideStereo(bool value);
			virtual bool SetLooseMidSideStereo(bool value);
			virtual bool SetApodization(String^ specification);
			virtual bool SetMaxLpcOrder(UInt32 value);
			virtual bool SetQlpCoeffPrecision(UInt32 value);
			virtual bool SetDoQlpCoeffPrecSearch(bool value);
			virtual bool SetDoEscapeCoding(bool value);
			virtual bool SetDoExhaustiveModelSearch(bool value);
			virtual bool SetMinResidualPartitionOrder(UInt32 value);
			virtual bool SetMaxResidualPartitionOrder(UInt32 value);
			virtual bool SetRiceParameterSearchDist(UInt32 value);
			virtual bool SetTotalSamplesEstimate(UInt64 value);
			virtual bool SetMetadata(array<Metadata::Prototype^>^ metadata);
		};

		public ref class File
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
			// Dummy
			virtual WriteStatus writeCallback(array<Byte>^ buffer, size_t bytes, UInt32 samples, UInt32 currentFrame) override;
			virtual void progressCallback(UInt64 bytesWritten, UInt64 samplesWritten, UInt32 framesWritten, UInt32 totalFramesEstimate);
		public:
			File();

			~File();

			virtual InitStatus Init(String^ fileName);
			virtual InitStatus InitOgg(String^ fileName);
		};
	}
}
