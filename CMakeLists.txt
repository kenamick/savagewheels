cmake_minimum_required (VERSION 2.8)
project(savagewheels) 

### App defaults
set(APP_ARCH "")
set(APP_VER_MAJ "1")
set(APP_VER_MIN "5")
set(APP_VER_PATCH "0")

### check for required libs
find_package(SDL REQUIRED)

### src
include_directories(src/)
include_directories(tinyxml/)

set (
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

# add SDL
set(SDL_LIB_NAME "libSDL.so")

target_link_libraries( 
	savagewheels
	${SDL_LIBRARY}
	SDLmain
)

find_library(
    SDL_LIBRARY_SO
    NAMES ${SDL_LIB_NAME}
    PATHS "/usr/lib" "/usr/local/lib/" ${SDL_LIBRARY}
    )

add_definitions(`sdl-config --cflags`)
add_definitions(`sdl-config --libs`)

message(STATUS "libSDL.so is in ${SDL_LIBRARY_SO}")

# check if sound support is required
if (NOT NO_SOUND)
  set(FMOD_LIB_NAME "libfmodex.so")

  # add FMod Ex (if present)
  find_library( 
    FMOD_LIBRARY
    NAMES ${FMOD_LIB_NAME}
    PATHS "/usr/lib" "/usr/local/lib/" ${FMOD_SO_PATH}
  )

  if (NOT FMOD_LIBRARY)
      message( STATUS "FModEx lib(${FMOD_LIB_NAME}) not found! Will compile without sound support." )
  else()
      add_definitions(-DWITH_FMOD)
      add_definitions(-I/usr/include/fmodex)
      target_link_libraries(savagewheels ${FMOD_LIBRARY})
      message( STATUS "FModEx lib(${FMOD_LIB_NAME}) at ${FMOD_LIBRARY}." )
  endif()

else()
  message( STATUS "NO_SOUND option found! Compiling without sound support ..." )
endif()

### additional defines
add_definitions(-DLINUX_BUILD)

message( STATUS "Build type is ${CMAKE_BUILD_TYPE}" )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-g)
  add_definitions(-D_DEBUG)
  add_definitions(-DDEBUG)
else()
  add_definitions(-O3 -DNDEBUG)
endif()

if (UNIX AND NOT WIN32)
  if (CMAKE_SIZEOF_VOID_P MATCHES "8")
    add_definitions(-DARCH_X64)
    set(APP_ARCH "x64")
    set(DEB_ARCH "amd64")
  else()
    add_definitions(-DARCH_X86)
    set(APP_ARCH "x86")
    set(DEB_ARCH "i386")
  endif()
endif()

### configure a header file to pass some of the CMake settings to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_SOURCE_DIR}/src/Config.h")
  
### copy required libs
add_custom_command(TARGET savagewheels POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    ${FMOD_LIBRARY} 
    $<TARGET_FILE_DIR:savagewheels>)        

add_custom_command(TARGET savagewheels POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    ${SDL_LIBRARY_SO}
    $<TARGET_FILE_DIR:savagewheels>)  

### Install targets
INSTALL(TARGETS savagewheels 
    RUNTIME DESTINATION "./")
INSTALL(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${FMOD_LIB_NAME}"
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
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")

set(CPACK_RESOURCE_FILE_LICENSE  "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README  "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
#set(CPACK_INSTALLED_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/bin;/.")
#set(CPACK_IGNORE_FILES 
#"~$"
#"^.*/.git/"
#"^${PROJECT_SOURCE_DIR}/bin/pref"
#"^${PROJECT_SOURCE_DIR}/bin/debug.html"
#)
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local/games/savagewheels")
set(CPACK_PACKAGE_EXECUTABLES "savagewheels" "Savage Wheels Game")

# Debian DEB package stuff
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCH}) 
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl1.2debian (>= 1.2.11)")

include (CPack)
