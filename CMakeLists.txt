cmake_minimum_required (VERSION 2.8)
project(savagewheels) 

### App defaults
set(APP_ARCH "")
set(APP_VER_MAJ "1")
set(APP_VER_MIN "5")
set(APP_VER_PATCH "0")

### check for required libs
find_package(SDL REQUIRED)

### sources
include_directories(src/)
include_directories(tinyxml/)

set(
	SOURCES 
	src/CBufferedReader.cpp
	src/Bindings.cpp 
	src/CDeadToy.cpp 
	src/CKdf.cpp 
	src/Csdl.cpp 
	src/Cswv_module.cpp 
	src/Cvehicle.cpp 
	src/Utils.cpp 
	src/CAnimation.cpp 
	src/Cgame.cpp 
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

add_executable (	
	savagewheels
	${SOURCES}
)

### add required libarires

## SDL

if (UNIX AND NOT WIN32)
	# Linux
	
	set(SDL_LIB_NAME "libSDL.so")
	target_link_libraries( 
		savagewheels
		${SDL_LIBRARY}
		SDLmain
	)
	
	find_library(
		SDL_LIBRARY_SO
		NAMES ${SDL_LIB_NAME}
		PATHS "/usr/lib" "/usr/local/lib/" ${SDL_LIBRARY} $ENV{SDLDIR} "${CMAKE_CURRENT_SOURCE_DIR}/libs"
		)	
else()
	# Windows
	
	set(SDL_LIB_NAME "SDL.dll")
	target_link_libraries( 
		savagewheels
		${SDL_LIBRARY}
	)
	add_definitions(-lpthread)
	include_directories(${SDL_INCLUDE_DIR})
	link_directories($ENV{SDLDIR}\\lib)
	
	set(SDL_LIBRARY_SO $ENV{SDLDIR}\\bin\\SDL.dll)
endif()

message(STATUS "libSDL.so/DLL is at ${SDL_LIBRARY_SO}")

add_custom_command(TARGET savagewheels POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different 
	${SDL_LIBRARY_SO}
	$<TARGET_FILE_DIR:savagewheels>)  

# check if sound support is available
if (NOT NO_SOUND)

	# check if path to FMod is defined
	if(NOT DEFINED ${FMOD_PATH})
		set(FMOD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs/fmod/api")
	endif()

	if (UNIX AND NOT WIN32)
		# Linux
		
		set(FMOD_LIB_NAME "libfmodex.so")
		set(FMOD_SO_NAME "libfmodex.so")
		
		add_definitions(-I/usr/include/fmodex)
	else()
		# Windows
		
		set(FMOD_LIB_NAME "libfmodex.a")
		if (CMAKE_SIZEOF_VOID_P MATCHES "8")
			set(FMOD_SO_NAME "fmodex64.dll")
		else()
			set(FMOD_SO_NAME "fmodex.dll")
		endif()
		
		add_definitions(-I${FMOD_PATH}/inc)
		link_directories(${FMOD_PATH}/lib)
	endif()

	# add FMod Ex (if present)
	find_library( 
		FMOD_LIBRARY
		NAMES ${FMOD_LIB_NAME}
		PATHS "/usr/lib" "/usr/local/lib/" "${FMOD_PATH}/lib"
	)
	
	find_library(
		FMOD_LIBRARY_SO_PATH
		NAMES ${FMOD_SO_NAME}	
		PATHS "/usr/lib" "/usr/local/lib/" "${FMOD_PATH}"
	)

	if (NOT FMOD_LIBRARY)
		message( STATUS "FModEx lib - ${FMOD_LIB_NAME} not found! Will compile without sound support." )
	else()
		message( STATUS "Found FModEx lib at ${FMOD_LIBRARY}. Found FModEx DLL at ${FMOD_LIBRARY_SO_PATH}." )
		
		add_definitions(-DWITH_FMOD)
		target_link_libraries(savagewheels ${FMOD_LIBRARY})
		
		add_custom_command(TARGET savagewheels POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different 
			${FMOD_LIBRARY_SO_PATH} 
			$<TARGET_FILE_DIR:savagewheels>)
			
		INSTALL(FILES
		    "${CMAKE_CURRENT_BINARY_DIR}/${FMOD_SO_NAME}"
			DESTINATION "./")
	endif()

else()
	message( STATUS "NO_SOUND option found! Compiling without sound support ..." )
endif()

### additional defines

if (UNIX AND NOT WIN32)
	add_definitions(-DLINUX_BUILD)
endif()

message( STATUS "Build type is ${CMAKE_BUILD_TYPE}" )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-g -D_DEBUG -DDEBUG)
else()
	add_definitions(-O3 -DNDEBUG)
endif()

if (CMAKE_SIZEOF_VOID_P MATCHES "8")
	add_definitions(-DARCH_X64)
	set(APP_ARCH "x64")
	set(DEB_ARCH "amd64")
else()
	add_definitions(-DARCH_X86)
	set(APP_ARCH "x86")
	set(DEB_ARCH "i386")
endif()

### configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_SOURCE_DIR}/src/Config.h")

### Install targets
INSTALL(TARGETS savagewheels 
    RUNTIME DESTINATION "./")
INSTALL(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${SDL_LIB_NAME}"
    "${CMAKE_CURRENT_SOURCE_DIR}/run.sh"
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/HISTORY"
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
#    "${CMAKE_CURRENT_SOURCE_DIR}/sw1.ico"
    DESTINATION "./")
INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
    DESTINATION "./"
    PATTERN "debug.html" EXCLUDE
    PATTERN "pref" EXCLUDE)
    
### build a CPack driven installer package
include (InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "TGZ")

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

# Advanced settings
if (UNIX AND NOT WIN32)
	set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
	set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local/games/savagewheels")
else()
	set(CPACK_CMAKE_GENERATOR "MinGW Makefiles")
	set(CPACK_PACKAGING_INSTALL_PREFIX "/savagewheels")
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

# Debian DEB package stuff
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCH}) 
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl1.2debian (>= 1.2.11)")

include (CPack)
