// Main.h  -  biblioteki, typeowe, constanti i tem podobni globalni definicii


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
#include <memory.h>
#include <exception>

#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/timeb.h>

#ifndef LINUX_BUILD

#	pragma comment( lib, "sdl.lib" )
#	pragma comment( lib, "sdlmain.lib" )
#	pragma comment( lib, "fmodvc.lib" )

#	define BACKSLASH	"\\"
#	define ASSERT		assert		// TODO: check if available

#else

//#	include <windows.h>

#	define BACKSLASH	"/"
#	define ASSERT		assert

#endif


#if !defined(_DEBUG) && !defined(DEBUG) && !defined(__DEBUG)
#	ifdef ASSERT
#	undef ASSERT
#	define ASSERT ;
#	endif
#endif

//#define ATODW(n) ((n[3] & 0x000000FF) | ((n[2] & 0x000000FF) << 4) | ((n[1] & 0x000000FF) << 8) | ((n[0] & 0x000000FF) << 16))
#define ATODW(n) ( ((((((n[3] & 0x000000FF) << 8) | n[2]) << 8) | n[1]) << 8) | n[0])
//#define DWTOA(n,a) sprintf(a, "%d", n);
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
#define VER_MAJ 1
#define VER_MIN 4

// --- global game defines
#define ART_FILE		"graphics/gfxdata.kdf"
#define BINDINGS_FILE	"bindings.xml"
#define LOAD_OK         (-1)
#define LOAD_FAILED     (0)
#define HRESULT(x)		( (x) == NULL : LOAD_OK ? LOAD_FAILED )

struct POINT 
{
	Uint16 x, y;
};

typedef std::string			String;
typedef std::ostringstream	OutputSStream;

// dobawqne na localnite headarni failove
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
