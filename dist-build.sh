#!/bin/sh

CMD="$1"
TAR="$(which tar)"
CUR_DIR="$(dirname $(readlink -f $0))"
BIN_PATH="$CUR_DIR/bin"
TMP_PATH="$CUR_DIR/dist-build.temp"
EXEC_PATH="$CUR_DIR/savagewheels"
LIBSDL_PATH="$CUR_DIR/libsdl"
VERMAJ=`perl -nle 'print $1 if /.*VER_MAJ\s(\d+).*/' src/Main.h`
VERMIN=`perl -nle 'print $1 if /.*VER_MIN\s(\d+).*/' src/Main.h`
VERSION="$VERMAJ.$VERMIN"

#for vr in `grep VER src/Main.h | awk '{ print $3 }' | tr -d \n\r\t`
#do
#	if [ "$VERMAJ" = "" ]; then
#		VERMAJ="$vr"
#	elif [ "$VERMIN" = "" ]; then
#		VERMIN="$vr"
#	fi
#echo $vr
#done

usage() {
	echo "dist-build.sh - Savage Wheels ${VERMAJ}.${VERMIN} distributable package build script"
	echo "Usage:"
	echo "	clean - clean files from previous package build"
	echo "	build - clean and create new package files"
	echo
}

cleanup() {
	echo "Cleaning up old files ..."

	if [ -e $TMP_PATH ]; then
		rm -rf $TMP_PATH
	fi

	echo "Cleanup done."
}

build() {
	echo "Building package ..."

	mkdir $TMP_PATH
 	if [ ! -e $TMP_PATH ]; then
		echo "Dist path could not be created - $TMP_PATH"
		exit
	fi
	if [ ! -e $EXEC_PATH ]; then
		echo "Executable was not found. Is the project build ?"
		exit
	fi

	cp $CUR_DIR/LICENSE $TMP_PATH

	cp $BIN_PATH/* $TMP_PATH -R
	cp $EXEC_PATH $TMP_PATH
	cp $LIBSDL_PATH/README*.txt $TMP_PATH

	rm $TMP_PATH/debug*.html -f
	rm $TMP_PATH/pref -f

	echo "Creating archive ..."
	
	PAK_NAME="savagewheels-$VERSION-linux.tar.gz"
	
	cd $TMP_PATH
	$TAR -czf $PAK_NAME --exclude=.svn *
	#mv $PAK_NAME $TMP_PATH -f
	cd ..

	echo "Build done."
	echo "You can find the $PAK_NAME archive in $TMP_PATH. Enjoy :)"
}

if [ "$1" = "clean" ]; then
	cleanup
elif [ "$1" = "build" ]; then
	cleanup
	build
else
	usage
fi
