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

usage() {
	echo "dist-build.sh - Savage Wheels ${VERMAJ}.${VERMIN} distributable package build script"
	echo "Usage:"
	echo "	clean - clean files from previous package build"
	echo "	build - clean and create new package files"
	echo
}

cleanup() {
	echo "Cleaning up old files ..."

	cd $BUILD_PATH
	make clean
	cd ..
	
	rm $BUILD_PATH/* -rf

	echo "Cleanup done."
}

build() {
	echo "Building package ..."
	
	if [ ! -e $CMAKE ]; then
		echo "CMake was not found - $CMAKE"
		exit	
	fi
	if [ ! -e $CPACK ]; then
		echo "CPack was not found - $CPACK"
		exit	
	fi	
	
	cd $BUILD_PATH
	$CMAKE cmake -G "Unix Makefiles" ../ \
            -DCMAKE_BUILD_TYPE:STRING=Release \
            -DCMAKE_BUILD_STATIC=1
	make -j$CPU_CORES
	
	$CPACK
	
	cd ..
	
	echo "Build done."
	echo "You can find the game packages in $BUILD_PATH. Enjoy :)"
}

if [ "$1" = "clean" ]; then
	cleanup
elif [ "$1EMPTY" = "EMPTY" ]; then
	cleanup
	build
else
	usage
fi
