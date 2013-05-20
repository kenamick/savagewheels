#!/bin/bash
# Desc: Savage Wheels Linux Build Script

CMD="$1"
CPU_CORES="2"
TAR="$(which tar)"
CUR_DIR="$(dirname $(readlink -f $0))"
BIN_PATH="$CUR_DIR/bin"
BUILD_PATH="$CUR_DIR/release"
CMAKE="$(which cmake)"
CPACK="$(which cpack)"
VERMAJ=`perl -nle 'print $1 if /.*VER_MAJ\s(\d+).*/' src/Main.h`
VERMIN=`perl -nle 'print $1 if /.*VER_MIN\s(\d+).*/' src/Main.h`
VERSION="$VERMAJ.$VERMIN"
#ARCH="$(uname -m)"
#TMP_PATH="$CUR_DIR/build/savagewheels-$VERSION-linux"

#if [ $ARCH = "x86_64" ]; then
#	TMP_PATH="$TMP_PATH-x64"
#else
#	TMP_PATH="$TMP_PATH-$ARCH"
#fi

usage() {
	echo "dist-build.sh - Savage Wheels ${VERMAJ}.${VERMIN} distributable package build script"
	echo "Usage:"
	echo "	clean - clean files from previous package build"
	echo "	build - clean and create new package files"
	echo
}

cleanup() {
	echo "Cleaning up old files ..."

	#if [ -e $TMP_PATH ]; then
	#	rm -rf $TMP_PATH
	#fi

	cd $BUILD_PATH
	make clean
	cd ..
	
	rm $BUILD_PATH/* -rf

	echo "Cleanup done."
}

build() {
	echo "Building package ..."

	mkdir $TMP_PATH
 	if [ ! -e $TMP_PATH ]; then
		echo "Dist path could not be created - $TMP_PATH"
		exit
	fi
	
	if [ ! -e $CMAKE ]; then
		echo "CMake was not found - $CMAKE"
		exit	
	fi
	if [ ! -e $CPACK ]; then
		echo "CPack was not found - $CPACK"
		exit	
	fi	
	
	cd $BUILD_PATH
	$CMAKE cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release
	make -j$CPU_CORES
	
	$CPACK
	
	cd ..
	
	echo "Build done."
	echo "You can find the game packages in $BUILD_PATH. Enjoy :)"
}

if [ "$1" = "clean" ]; then
	cleanup
#elif [ "$1" = "build" ]; then
else
	cleanup
	build
#else
#	usage
fi
