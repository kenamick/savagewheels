cmake_minimum_required (VERSION 2.8)
project(savagewheels) 

### Defaults
set(APP_ARCH "")
set(APP_VER_MAJ "1")
set(APP_VER_MIN "5")
set(APP_VER_PATCH "0")

### check for required libs
find_package(SDL REQUIRED)

### Install targets (see: http://www.cmake.org/Wiki/CMake_RPATH_handling)
if (NOT INSTALL_DIR)
    set(INSTALL_DIR "./")
endif()

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# add the automatically determined parts of the RPATH
SET(CMAKE_INSTALL_RPATH "${INSTALL_DIR}/")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

### sources
include_directories(src/)
include_directories(tinyxml/)

set(
	SOURCES 
	src/CBufferedReader.cpp
	src/Bindings.cpp 
	src/CDeadToy.cpp 
	src/CKdf.cpp 
	src/CSdl.cpp 
	src/CSwv_module.cpp 
	src/CVehicle.cpp 
	src/Utils.cpp 
	src/CAnimation.cpp 
	src/CGame.cpp 
	src/CMainMenu.cpp 
	src/CSounds.cpp
	src/CTimer.cpp
	src/Main.cpp
	tinyxml/tinystr.cpp
	tinyxml/tinyxml.cpp
	tinyxml/tinyxmlerror.cpp
	tinyxml/tinyxmlparser.cpp	
)

### specify executable

set(RES_FILES "")
if(MINGW)
    # include a Windows resource file
	set(RES_FILES "src/res.rc")
	set(CMAKE_RC_COMPILER_INIT windres)
	ENABLE_LANGUAGE(RC)
	SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
	
	add_executable(savagewheels ${SOURCES} ${RES_FILES})	
else()
	add_executable(savagewheels ${SOURCES})	
endif()

### add required libarires

## libstdc runtime on Windows
if (WIN32)
	# Static linkage of the gcc libs
	#add_definitions(-static-libgcc)
	#add_definitions(-static-libstdc++)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

## SDL
if (UNIX AND NOT WIN32)
	# Linux
	
#	set(SDL_LIB_NAME "libSDL.so")
#	find_library(
#		SDL_LIBRARY_SO
#		NAMES ${SDL_LIB_NAME}
#		PATHS "/usr/lib" "/usr/local/lib/"
#		HINTS $ENV{SDLDIR} ${SDL_LIBRARY} "${CMAKE_CURRENT_SOURCE_DIR}/libs")
    add_definitions(`sdl-config --cflags`)
    add_definitions(`sdl-config --libs`)	
else()
	# Windows
	
	set(SDL_LIB_NAME "SDL.dll")
	
	add_definitions(-lpthread)
	include_directories(${SDL_INCLUDE_DIR})
	link_directories($ENV{SDLDIR}\\lib)
	set(SDL_LIBRARY_SO $ENV{SDLDIR}\\bin\\SDL.dll)
	
	message(STATUS "Found libSDL.so/DLL at ${SDL_LIBRARY}")
	
    # copy the SDL DLL file to the install dir
    add_custom_command(TARGET savagewheels POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    	${SDL_LIBRARY_SO}
    	$<TARGET_FILE_DIR:savagewheels>)  
    	
    INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/SDL.dll" DESTINATION ${INSTALL_DIR})
		
endif()

## SOUND libraries
if (SOUND STREQUAL "NO")
    message(STATUS "Found SOUND=NO option. Compiling without sound support ...")
else()
    # FModEx API

	# check if path to FMod is defined
	if(NOT DEFINED ${FMOD_PATH})
		set(FMOD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/fmod/api")
	endif()

    add_definitions(-I${FMOD_PATH}/inc)
    link_directories(${FMOD_PATH}/lib)

	if (UNIX AND NOT WIN32)
		# Linux
		
		if (CMAKE_SIZEOF_VOID_P MATCHES "8" AND NOT M32)
			set(FMOD_LIB_NAME "libfmodex64.so")
			set(FMOD_SO_NAME "libfmodex64.so")
		else()
			set(FMOD_LIB_NAME "libfmodex.so")
			set(FMOD_SO_NAME "libfmodex.so")
		endif()		
		
		# just in case check also in the system folders
		add_definitions(-I/usr/include/fmodex)
	else()
		# Windows
		
		set(FMOD_LIB_NAME "libfmodex.a")
		if (CMAKE_SIZEOF_VOID_P MATCHES "8" AND NOT M32)
			set(FMOD_SO_NAME "fmodex64.dll")
		else()
			set(FMOD_SO_NAME "fmodex.dll")
		endif()
	endif()

	# add FMod Ex (if present)
	find_library( 
		FMOD_LIBRARY
		NAMES ${FMOD_LIB_NAME}
		PATHS "/usr/lib" "/usr/local/lib/"
		HINTS "${FMOD_PATH}/lib")
	
	find_library(
		FMOD_LIBRARY_SO_PATH
		NAMES ${FMOD_SO_NAME}	
		PATHS "/usr/lib" "/usr/local/lib/"
		HINTS "${FMOD_PATH}" "${FMOD_PATH}/lib")

	if (NOT FMOD_LIBRARY)
		message(STATUS "FModEx lib - ${FMOD_LIB_NAME} not found! Will compile without sound support.")
	else()
		message(STATUS "Found FModEx lib at ${FMOD_LIBRARY}. Found FModEx DLL at ${FMOD_LIBRARY_SO_PATH}.")
		
		add_definitions(-DWITH_FMOD)
		
		add_custom_command(TARGET savagewheels POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			${FMOD_LIBRARY_SO_PATH} 
			$<TARGET_FILE_DIR:savagewheels>)
			
		INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/${FMOD_SO_NAME}" 
		    DESTINATION ${INSTALL_DIR})
	endif()

endif()

### link required libraries

target_link_libraries(savagewheels ${SDL_LIBRARY} ${FMOD_LIBRARY})

### additional defines

if (UNIX AND NOT WIN32)
	add_definitions(-DLINUX_BUILD)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-g -D_DEBUG -DDEBUG) # -Wextra -Wall
else()
	add_definitions(-O3 -DNDEBUG)
endif()
message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")

### set architecture

if (CMAKE_SIZEOF_VOID_P MATCHES "8" AND NOT M32)
	add_definitions(-DARCH_X64)
	set(APP_ARCH "x64")
	set(DEB_ARCH "amd64")
else()
	add_definitions(-DARCH_X86)
	set(APP_ARCH "x86")
	set(DEB_ARCH "i386")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
endif()

### Configure compile time header file 
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_SOURCE_DIR}/src/Config.h")
  
INSTALL(TARGETS savagewheels RUNTIME DESTINATION ${INSTALL_DIR})
INSTALL(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/run.sh" DESTINATION ${INSTALL_DIR})
INSTALL(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/HISTORY"
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    DESTINATION ${INSTALL_DIR})
INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    DESTINATION ${INSTALL_DIR}
    PATTERN "debug.html" EXCLUDE
    PATTERN "pref" EXCLUDE)
    
### build a CPack driven installer package
include (InstallRequiredSystemLibraries)

if (NOT CPACK_GENERATOR AND (UNIX AND NOT WIN32))
	set(CPACK_GENERATOR "STGZ")
else()
	set(CPACK_GENERATOR "TGZ")
endif()

set(CPACK_PACKAGE_NAME "SavageWheels")
set(CPACK_PACKAGE_VENDOR "Petar Petrov")
set(CPACK_PACKAGE_CONTACT "petar.petrov.georgiev(at-)gmail[dot)com")
set(CPACK_PACKAGE_VERSION_MAJOR "${APP_VER_MAJ}")
set(CPACK_PACKAGE_VERSION_MINOR "${APP_VER_MIN}")
set(CPACK_PACKAGE_VERSION_PATCH "${APP_VER_PATCH}")
set(PCKVER "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )
set(CPACK_PACKAGE_VERSION ${PCKVER})

set(CPACK_PACKAGE_FILE_NAME "savagewheels-${PCKVER}-${CMAKE_SYSTEM_NAME}-${APP_ARCH}")
#set(CPACK_PACKAGE_INSTALL_DIRECTORY "savagewheels-${PCKVER}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Debian DEB package stuff
if (${CPACK_GENERATOR} STREQUAL "DEB")
	set(CPACK_PACKAGING_INSTALL_PREFIX "/opt")
	set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCH}) 
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libstdc++6, libsdl1.2debian (>= 1.2.11)")
endif()

set(CPACK_RESOURCE_FILE_LICENSE  "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README  "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
#set(CPACK_INSTALLED_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/bin;/.")
#set(CPACK_IGNORE_FILES 
#"~$"
#"^.*/.git/"
#"^${PROJECT_SOURCE_DIR}/bin/pref"
#"^${PROJECT_SOURCE_DIR}/bin/debug.html"
#)
set(CPACK_PACKAGE_EXECUTABLES "savagewheels" "Savage Wheels Game")

include (CPack)
