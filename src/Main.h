/*
    Copyright (c) 2003-2010 KenamicK Entertainment

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef __CMAIN
#define __CMAIN

#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdarg>
#include <cassert>
#include <exception>

#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/timeb.h>

#ifdef LINUX_BUILD
#	define BACKSLASH	"/"
#else
#	pragma comment( lib, "sdl.lib" )
#	pragma comment( lib, "sdlmain.lib" )
#	pragma comment( lib, "fmodvc.lib" ) // MSVC
#	define BACKSLASH	"\\"
#endif

#define ASSERT	assert

#if !defined(_DEBUG) && !defined(DEBUG) && !defined(__DEBUG)
#	ifdef ASSERT
#	undef ASSERT
#	define ASSERT(x)
#	endif
#endif

#define ATOQW(n) ( (((((((((((((((n[7] & 0x00000000000000FF) << 8) | n[6]) << 8) | n[5]) << 8) | n[4]) << 8) | n[3]) << 8) | n[2]) << 8) | n[1]) << 8) | n[0] ) )
#define ATODW(n) ( ((((((n[3] & 0x000000FF) << 8) | n[2]) << 8) | n[1]) << 8) | n[0])
#define ATOW(n) ( ((n[0] & 0x00FF) << 8) | n[1])
#define DWTOA(n,a) sprintf((char *)a, "%d", n);

// --- lib includes
#include "SDL.h"
//#include "sdl/sdl_ttf.h"
#ifdef WITH_FMOD
  #include "fmod.h"
  #include "fmod_errors.h"
#endif
// ---

#ifndef UINT_MAX
#define UINT_MAX 0xffffffff
#endif

#ifndef INT_MAX
#define INT_MAX	 0x7fffffff //2147483647
#endif

// --- version
#include "Config.h"

// --- global game defines
#define ART_FILE	"graphics/gfxdata.kdf"
#define BINDINGS_FILE	"bindings.xml"
#define LOAD_OK         (-1)
#define LOAD_FAILED     (0)
#define HRESULT(x)	( (x) == NULL : LOAD_OK ? LOAD_FAILED )
#define NLPTR_SURF	0 //((SDL_Surface *)NULL)

#ifdef WITH_FMOD
#define FM_OK(x) CSdl::IsFModOK(x)
#else
#define FM_OK(x)
#endif

struct POINT 
{
	Uint16 x, y;
};

typedef std::string			String;
typedef std::ostringstream	OutputSStream;

//#include "pstdint.h" // portable types
#include "Utils.h"
#include "CKdf.h"
#include "Csdl.h"
#include "Cswv_module.h"
#include "CAnimation.h"
#include "CMainMenu.h"
#include "CTimer.h"
#include "Cvehicle.h" 
#include "CSounds.h"
#include "CDeadToy.h"
#include "Bindings.h"
#include "Cgame.h"
#include "CBufferedReader.h"

#endif
