//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by version.rc
//

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif


#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)


#define VERSION_MAJOR               1
#define VERSION_MINOR               2
#define VERSION_REVISION            1
#define VERSION_BUILD               0

#define VER_FILE_DESCRIPTION_STR    "2DTest Application"

#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_STR        STRINGIZE(VERSION_MAJOR)        \
                                    "." STRINGIZE(VERSION_MINOR)    \
                                    "." STRINGIZE(VERSION_REVISION) \
                                    "." STRINGIZE(VERSION_BUILD)
#define VER_FILE_VERSION_STR_       _T(STRINGIZE(VERSION_MAJOR))        \
                                    _T(".") _T(STRINGIZE(VERSION_MINOR))    \
                                    _T(".") _T(STRINGIZE(VERSION_REVISION)) \
                                    _T(".") _T(STRINGIZE(VERSION_BUILD))
#define VER_PRODUCTNAME_STR         "2DTest"
#define VER_PRODUCT_VERSION         VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR     VER_FILE_VERSION_STR
#define VER_ORIGINAL_FILENAME_STR   "2DTest.exe"
#define VER_INTERNAL_NAME_STR       "2DTest"
#define VER_COPYRIGHT_STR           "Copyright (C) 2018 Mikhalich"

#ifdef _DEBUG
  #define VER_VER_DEBUG             VS_FF_DEBUG
#else
  #define VER_VER_DEBUG             0
#endif

#define VER_FILEOS                  VOS_NT_WINDOWS32
#define VER_FILEFLAGS               VER_VER_DEBUG
#define VER_FILETYPE                VFT_APP

