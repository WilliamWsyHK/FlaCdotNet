# Guideline

**This project uses C# 6.0 features, such as nameof(FlaCdotNet), $"{decoder.Status}", etc.**
**If the compiler complains, either you don't got C# 6.0 compiler, or you did not do it correctly**

1. Refer to official [FLAC API](http://xiph.org/flac/api/)
2. Do NOT use original verbose name, nor **ALL_CAP** name for type names, unless there is no other better options (e.g. use *Metadata* instead of *FLAC__StreamMetadata*)
    * Enum values is also a good example for this: use *MetadataPictureType.FileIcon* instead of **ALL_CAP** *::FLAC__STREAMMETADATA_PICTURE_TYPE_FILE_ICON* (Are you sure you want the latter one in C#?)
3. Use C#/.NET naming convention!
4. Unless really necessary to access class members using *internal* / *protected internal*, mark the visibility of these members to *protected* / *private* only 
5. File naming convention:
    * FunctionalType.Class.h
      * Metadata.SeekTable.h
    * FunctionalType.Class.SubElement.cpp
      * Metadata.SeekTable.cpp [Only contains SeekTable]
      * Metadata.SeekTable.SeekPoint.cpp [Only contains SeekTable.SeekPoint]
6. If any code can be referenced in libFLAC++ for warpping libFLAC codes, then use them!
    * Codes in this project just like translating C++ codes to .NET ones, and make sure the names are in C#/.NET conventions.
    * **Property should be added whenever possible!**
