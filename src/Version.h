/*
See License.txt for details about distribution and modification.
*/

#ifndef NOTEPAD2_VERSION_H
#define NOTEPAD2_VERSION_H

#include "VersionRev.h"

#ifndef _T
	#if !defined(ISPP_INVOKED) && (defined(UNICODE) || defined(_UNICODE))
		#define _T(text) L##text
	#else
		#define _T(text) text
	#endif
#endif

#define _QUOTEME(x) #x
#define QM(x) _QUOTEME(x)
#define _QUOTEME2(x, y) _QUOTEME(x##y)
#define QM2(x, y) _QUOTEME2(x, y)
#define _QUOTEME3(x, y, z) _QUOTEME(x##y##z)
#define QM3(x, y, z) _QUOTEME3(x, y, z)
#define _QUOTEME4(x, y, z, u) _QUOTEME(x##y##z##u)
#define QM4(x, y, z, u) _QUOTEME4(x, y, z, u)

#ifndef CURR_VERSION
#define CURR_VERSION 0.1.0
#endif
#ifndef CURR_VERSION_COMMA
#define CURR_VERSION_COMMA 0,1,0
#endif

// VER_QUALIFIER allows people who recompile SumatraPDF to add
// a distinguishing string at the end of the version number
// (e.g. version 2.3.2z or 2.4opt or 2.5x64)
#if !defined(VER_QUALIFIER) && defined(DEBUG)
// adds " (dbg)" after the version in debug builds
#define VER_QUALIFIER \x20(dbg)
#endif

// version as displayed in UI and included in resources
#ifndef PRE_RELEASE_VER
 #ifndef VER_QUALIFIER
  #define CURR_VERSION_STRA QM(CURR_VERSION)
 #else
  #define CURR_VERSION_STRA QM2(CURR_VERSION, VER_QUALIFIER)
 #endif
 #define VER_RESOURCE_STR  CURR_VERSION_STRA
 #define VER_RESOURCE      CURR_VERSION_COMMA,0
 #define UPDATE_CHECK_VER  TEXT(QM(CURR_VERSION))
#else
 #ifndef VER_QUALIFIER
   #define CURR_VERSION_STRA QM3(CURR_VERSION, ., PRE_RELEASE_VER)
   #define VER_RESOURCE_STR  QM3(CURR_VERSION, .0., PRE_RELEASE_VER)
 #else
   #define CURR_VERSION_STRA QM4(CURR_VERSION, ., PRE_RELEASE_VER, VER_QUALIFIER)
   #define VER_RESOURCE_STR  QM4(CURR_VERSION, .0., PRE_RELEASE_VER, VER_QUALIFIER)
 #endif
 #define VER_RESOURCE      CURR_VERSION_COMMA,PRE_RELEASE_VER
 #define UPDATE_CHECK_VER  TEXT(QM(PRE_RELEASE_VER))
#endif
#define CURR_VERSION_STR TEXT(CURR_VERSION_STRA)

#define DO_STRINGIFY(x) _T(#x)
#define STRINGIFY(x) DO_STRINGIFY(x)


#define MY_APPNAME L"velociraptor8"
#define VERSION_FILEVERSION  VER_RESOURCE
#define VERSION_LEGALCOPYRIGHT_SHORT L"Copyright © 2004-2014"
#define VERSION_LEGALCOPYRIGHT_LONG L"© Florian Balmer 2004-2014"
#define VERSION_AUTHORNAME L"Krzysztof Kowalczyk"
#define VERSION_WEBPAGEDISPLAY L"flo's freeware - http://www.flos-freeware.ch"
#define VERSION_EMAILDISPLAY L"kkowalczyk@gmail.com"
#define VERSION_COMPANYNAME L"Florian Balmer et al."
#define VERSION_MODPAGEDISPLAY "http://blog.kowalczyk.info/software/velociraptor8/"

#if defined(_WIN64)
#define VERSION_FILEVERSION_LONG L"velociraptor8 (64-bit) " CURR_VERSION_STR
#else
#define VERSION_FILEVERSION_LONG L"velociraptor8 " CURR_VERSION_STR
#endif

// Compiler specific
#if defined(WDK_BUILD)
#if _MSC_VER == 1600
#if (_MSC_FULL_VER >= 160040219)
#define VERSION_COMPILER L"WDK (MSVC 2010 SP1)"
#else
#define VERSION_COMPILER L"WDK (MSVC 2010)"
#endif
#elif _MSC_VER == 1500
#if (_MSC_FULL_VER == 150030729)
#define VERSION_COMPILER L"WDK"
#else
#define VERSION_COMPILER L"WDK (version unknown)"
#endif
#endif
#elif defined(_MSC_VER)
#if _MSC_VER == 1800
#if (_MSC_FULL_VER == 180030723)
#define VERSION_COMPILER L"MSVC 2013 Update 3"
#elif(_MSC_FULL_VER == 180030501)
#define VERSION_COMPILER L"MSVC 2013 Update 2"
#elif(_MSC_FULL_VER < 180021005)
#define VERSION_COMPILER L"MSVC 2013 Preview/Beta/RC"
#else
#define VERSION_COMPILER L"MSVC 2013"
#endif
#elif _MSC_VER == 1700
#if (_MSC_FULL_VER == 170061030)
#define VERSION_COMPILER L"MSVC 2012 Update 4"
#elif(_MSC_FULL_VER == 170060610)
#define VERSION_COMPILER L"MSVC 2012 Update 3"
#elif(_MSC_FULL_VER == 170060315)
#define VERSION_COMPILER L"MSVC 2012 Update 2"
#elif(_MSC_FULL_VER == 170051106)
#define VERSION_COMPILER L"MSVC 2012 Update 1"
#elif(_MSC_FULL_VER < 170050727)
#define VERSION_COMPILER L"MSVC 2012 Beta/RC/PR"
#else
#define VERSION_COMPILER L"MSVC 2012"
#endif
#elif _MSC_VER == 1600
#if (_MSC_FULL_VER >= 160040219)
#define VERSION_COMPILER L"MSVC 2010 SP1"
#else
#define VERSION_COMPILER L"MSVC 2010"
#endif
#elif _MSC_VER == 1500
#if (_MSC_FULL_VER >= 150030729)
#define VERSION_COMPILER L"MSVC 2008 SP1"
#else
#define VERSION_COMPILER L"MSVC 2008"
#endif
#else
#define VERSION_COMPILER L"MSVC (version unknown)"
#endif
#else
#define VERSION_COMPILER L"(Unknown compiler)"
#endif

#endif // NOTEPAD2_VERSION_H
