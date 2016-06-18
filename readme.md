# Welcome to FlaCdotNet!

Please consider donate this project to support continue development.  
[![Donate](https://img.shields.io/badge/Donate-PayPal-blue.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=76EM5QKU7RP6W)

This is my first Open Source Project in my favourite .NET environment.  
The project needs to use libFLAC.lib & libFLAC++.lib to compile the libFlaCdotNet.dll.  
As FlaC is a **Free Lossless Audio Codec**, this **FlaCdotNet** will also be using the same license too.  

Please refer to the project and see the license files.  

Finally, I appreciate any improvement for this project, please report bugs if you find anything wrong (even spelling mistake :smiley:); or want something to add;  
If you want to contribute codes, *then please follow the syntax guideline (Guideline.md)* for this project!  

## Example
Use the .NET convention to code just like using libFLAC++  
``` C#
encoder.Init();
encoder.SetMetadata(metadata);
```

## How to Compile
You need to download the source code from [FLAC official site](http://xiph.org/flac/), Ogg library ([xiph.org download site](http://xiph.org/downloads/)) for FLAC compiled with Ogg support.  
Finally, put these *.h files in include folder  
* FLAC
  * all.h
  * assert.h
  * callback.h
  * export.h
  * format.h
  * metadata.h
  * ordinals.h
  * stream_decoder.h
  * stream_encoder.h
* FLAC++
  * all.h
  * decoder.h
  * encoder.h
  * export.h
  * metadata.h
  
and these *.lib files in lib folder  
* Win32/x64
  * Debug/Release
    * libFLAC_static.lib
    * libFLAC++_static.lib
    * libogg_static.lib
    * win_utf8_io_static.lib

## Important
This project only tested in x86, for both encoding and decoding parts.  
(Test cases are from example_cpp_encode_file/example_cpp_decode_file projects)  
This project expects you to add Ogg support. (Although created unit test case for ogg, it is not actually tested.)  

## **Some Trivia**
There are some problems found in Unit test for decoder, which cannot be passed, but the generated file is ~100% same as the original test from FLAC.  
Those are the stream info min/max frame size, and unknown type (used application instead).  
See the source code and the generated file for details.  

# Also Important Note
Use this kind of code to change values  
``` C#
using FlaCdotNet;
using FlaCdotNet.Metadata;
...
var point = new SeekTable.SeekPoint();
point.SampleNumber = ulong.MaxValue;
point.StreamOffset = 1000;
point.FrameSamples = streamInfo.MinBlockSize;
seekTable.InsertPoint(1, point);
...
```
to replace this (since the following example cannot update values as desired)  
``` C#
using FlaCdotNet;
using FlaCdotNet.Metadata;
...
//seekTable.NumPoints = 2;
seekTable.TemplateAppendPlaceholders(2);
seekTable[1].SampleNumber = ulong.MaxValue;
seekTable[1].StreamOffset = 1000;
seekTable[1].FrameSamples = streamInfo.MinBlockSize;
...
```

# Say hello to the contributer(s)
William WONG (that's me :kissing:)  
[Hope I could see your name here!]  

## License
Although this project is free to use, for any commercial use, please contact me.  

FlaCdotNet - Wrapper of FLAC for .NET Environment  
Copyright (C) 2016-      WONG, William Sung Yu  
*********************************************  
FLAC - Free Lossless Audio Codec  
Copyright (C) 2001-2009  Josh Coalson  
Copyright (C) 2011-2014  Xiph.Org Foundation  
  
This file is part the FLAC project.  FLAC is comprised of several  
components distributed under different licenses.  The codec libraries  
are distributed under Xiph.Org's BSD-like license (see the file  
COPYING.Xiph in this distribution).  All other programs, libraries, and  
plugins are distributed under the LGPL or GPL (see COPYING.LGPL and  
COPYING.GPL).  The documentation is distributed under the Gnu FDL (see  
COPYING.FDL).  Each file in the FLAC distribution contains at the top the  
terms under which it may be distributed.  
  
Since this particular file is relevant to all components of FLAC,  
it may be distributed under the Xiph.Org license, which is the least  
restrictive of those mentioned above.  See the file COPYING.Xiph in this  
distribution.  
