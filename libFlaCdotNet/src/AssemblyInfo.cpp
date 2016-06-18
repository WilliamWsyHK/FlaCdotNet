// AssemblyInfo.cpp

/* libFlaCppCliHelper - Wrapper of libFLAC++ for libFlaCSharp
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

#include "stdafx.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Permissions;

//
// 組件的一般資訊是由下列的屬性集控制。
// 變更這些屬性的值即可修改組件的相關
// 資訊。
//
[assembly:AssemblyTitleAttribute(L"libFlaCdotNet")];
[assembly:AssemblyDescriptionAttribute(L"")];
[assembly:AssemblyConfigurationAttribute(L"")];
[assembly:AssemblyCompanyAttribute(L"")];
[assembly:AssemblyProductAttribute(L"libFlaCdotNet")];
[assembly:AssemblyCopyrightAttribute(L"Copyright (c) WONG, William Sung Yu 2016")];
[assembly:AssemblyTrademarkAttribute(L"")];
[assembly:AssemblyCultureAttribute(L"")];

//
// 組件的版本資訊由下列四個值所組成: 
//
//      主要版本
//      次要版本
//      組建編號
//      修訂編號
//
// 您可以指定所有的值，也可以依照以下的方式，使用 '*' 將修訂和組建編號
// 指定為預設值: 

[assembly:AssemblyVersionAttribute("1.0.*")];

[assembly:ComVisible(false)];

[assembly:CLSCompliantAttribute(true)];