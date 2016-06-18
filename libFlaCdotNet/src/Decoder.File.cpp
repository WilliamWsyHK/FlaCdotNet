// Decoder.File.cpp

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
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace FlaCdotNet
{
	namespace Decoder
	{
		File::File()
			: Stream()
		{
			decoder_->isFile_ = true;
		}

		File::~File()
		{
			if (!disposedValue)
			{
				this->!File();

				disposedValue = true;
			}
		}

		File::!File()
		{
		}

		InitStatus File::Init(String^ fileName)
		{
			//array<Byte>^ bytes = Encoding::UTF8->GetBytes(fileName + "\0");
			InitStatus result;

			{
				//pin_ptr<Byte> b = &bytes[0];

				//result = static_cast<InitStatus>(decoder_->init(reinterpret_cast<const char*>(b)));

				pin_ptr<const wchar_t> wch = PtrToStringChars(fileName);
#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif
				FILE* f = _wfopen(wch, L"rb");
#ifdef _MSC_VER
#pragma warning (default: 4996)
#endif
				result = static_cast<InitStatus>(decoder_->init(f));
			}
			return result;
		}

		InitStatus File::InitOgg(String^ fileName)
		{
			//array<Byte>^ bytes = Encoding::UTF8->GetBytes(fileName + "\0");
			InitStatus result;

			{
				//pin_ptr<Byte> b = &bytes[0];

				//result = static_cast<InitStatus>(decoder_->init_ogg(reinterpret_cast<const char*>(b)));

				pin_ptr<const wchar_t> wch = PtrToStringChars(fileName);
#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif
				FILE* f = _wfopen(wch, L"rb");
#ifdef _MSC_VER
#pragma warning (default: 4996)
#endif
				result = static_cast<InitStatus>(decoder_->init_ogg(f));
			}
			return result;
		}

		ReadStatus File::readCallback(array<Byte>^ buffer, size_t% bytes)
		{
			throw gcnew InvalidProgramException();
		}
	}
}
