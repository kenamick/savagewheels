_.o0 Savage Wheels installation instructions 0o._
===================================================

_Last updated on 14 Sep 2015_

# Supported platforms

  * Linux x86 or x64 - Tested on Archlinux, Ubuntu and Debian.
  * Windows x86 or x64 - Tested on Windows XP, Vista and 2008R2.

# Required files

Get the latest source code from GitHub:

    git clone git://github.com/petarov/savagewheels.git savagewheels

Get the data files archive `savagewheels-gamedata.tar.gz` from 
https://code.google.com/p/savagewheels. 

Extract the downloaded archive into the `bin/` folder. The archive should contain the following folders: 
  * autos/
  * graphics/
  * sounds/

# Installation on Linux platforms

Arch Linux users may use the [PKGBUILD](https://aur.archlinux.org/packages/savagewheels/) in Arch User Repository (AUR).

## Requirements

  * cmake >= `2.8`
  * If building on Ubuntu, install the package `build-essential`.
  * libSDL >= `1.2.12` - You should install it using your package manager. (`libsdl1.2-dev`)
  * For music support either of:
    - SDL_Mixer >= `1.2.12` - You should install it using your package manager (`libsdl-mixer1.2-dev`).
    - FMod Ex API `4.x` - Install using your package manager or get the package from FMod's website.

## Compile & Build

 - Install the libSDL package using the package manager on your Linux distribution.
 - Install one of the supported sound libraries. See `Sound libraries` below.	
 - Run the build script:
   	
```bash
   	$ ./build.sh
````

 - Go to the `release/` subfolder. Extract the produced archive file, e.g., `savagewheels-1.6.0-Linux-x64.sh`, to a desired location.
 - To start the game run:

```bash	
	$ ./savagewheels  
```	
or
```bash	
	$ ./run.sh
````
	
The `run.sh` script will attempt to set the `LD_LIBRARY_PATH` environment variable in case you are missing the libSDL or FModEx shared object libraries.
   
If you experience problems with the automated `build.sh` script or you would like to change build options, you can build manually with CMake.
   
 - Go to the release/ sub folder and run:
	
```bash	
	$ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release
```

 - Build with:
```bash	
	$ make
```
 - Create the installation package with:
```bash	
	$ cpack
```

### Sound libraries
#### SDL_mixer

Install the SDL_mixer library using your package manager.

#### FMod Ex

Install the FMod Ex package using the package manager on your Linux distribution or get the package from FMod's website. 
If you choose the second option, you will need to copy the `api/` folder from FMod's installation package into the `libs/` subfolder of the project.
	
When you are running CMake, specify the SOUND=FMOD option, e.g., 

	$ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release -DSOUND=FMOD	

#### No sound

To build without sound & music run:

	$ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release -DSOUND=NO

### Compile 32-bit version on 64-bit Linux

This requires that you have the `gcc-multilib` package installed on your system. You will also need to have 32-bit versions of `libSDL.so` and `libfmodex.so`.
	
After all required packages are installed build the makefile by running:

	$ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release -DM32=1

## Packaging

After the build process has finished, a self-extracting STGZ file may be created by running:

	$ cpack

### Debian
	
Debian and Ubuntu users may run:
	
	$ cpack -G DEB
	
This will create a `.deb` package that you can laster install. E.g.,

	$ dpkg -i savagewheels-1.5.0-Linux-x64.deb

# Installation on Windows

## Requirements

  * MinGW (Minimalist GNU for Windows). I have only tested with [Mingw32](http://www.mingw.org/).
  * CMake >= `2.8.x`
  * libSDL >= `1.2.12` - Download the Development Libraries archive from http://www.libsdl.org/download-1.2.php.
  * For music support either of:
    - SDL_Mixer >= `1.2.12` - Download the binary `SDL_mixer-1.2.x-win32.zip` package and the source `SDL_mixer-1.2.x.zip` package from http://www.libsdl.org/projects/SDL_mixer/.
    - FMod Ex API 4.x - Download and install the FMOD Ex API installer EXE from www.fmod.org.
	
## Compile & Build

  - Extract the contents of SDL 1.2.x package into the libs/ subfolder. Set the environment variable `SDLDIR=<SDL-directory-in-libs>`.
	
  - Install one of the supported sound libraries. See `Sound libraries` below.

  - Run CMake manually. Go to the release/ sub folder and run.

	> cmake -G "MinGW Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release

  - To build run: 
  
	> mingw32-make
   
  - Create the installation package with:
  
	> cpack

  - Extract the produced archive file, e.g., `savagewheels-1.6.0-Windows-x86.tar.gz` to a desired folder.
	
  - To start the game run:
  
	> savagewheels  

### Sound libraries
#### SDL_mixer
	
Copy the contents of the `SDL_mixer-1.2.x-win32` binary package to `<SDL-directory-in-libs>/lib`.
	
Copy the `SDL_mixer.h header` file from `SDL_mixer-1.2.x.zip` sources package to `<SDL-directory-in-libs>/include`. 
	
#### FMod Ex

Install the FMod Ex API and copy the `api/` folder from FMod's installation into the `libs/` subfolder of the project.
	
When you are running `cmake`, specify the `SOUND=FMOD` option, e.g.,

	> cmake -G "MinGW Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release -DSOUND=FMOD
	
#### No sound

To build without sound & music run:
	
	> cmake -G "MinGW Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release -DSOUND=NO	
	
## Packaging

After the build process has finished, a self-extracting STGZ file may be created by running:

	> cpack
