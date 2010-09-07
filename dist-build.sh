#!/bin/sh

CMD="$1"
TAR="$(which tar)"
CUR_DIR="$(dirname `readlink -f $0)`)"
BIN_PATH="$CUR_DIR/bin"
TMP_PATH="$CUR_DIR/dist-build.temp"
EXEC_PATH="$CUR_DIR/savagewheels"
LIBSDL_PATH="$CUR_DIR/libsdl"
VERMAJ=""
VERMIN=""

for vr in `grep VER src/Main.h | awk '{ print $3 }' | tr -d \n\r\t`
do
	if [ "$VERMAJ" = "" ]; then
		VERMAJ="$vr"
	elif [ "$VERMIN" = "" ]; then
		VERMIN="$vr"
	fi
echo $vr
done

echo $VERSION

usage() {
	echo "dist-build.sh - Savage Wheels ${VERMAJ}.${VERMIN} build script"
	echo "Usage:"
	echo "	clean"
	echo "	build"
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

	cp $CUR_DIR/LICENSE $TMP_PATH

	cp $BIN_PATH/* $TMP_PATH -R
	cp $EXEC_PATH $TMP_PATH
	cp $LIBSDL_PATH/README*.txt $TMP_PATH

	rm $TMP_PATH/debug*.html -f
	rm $TMP_PATH/pref -f

	echo "Creating archive ..."
	
	PAK_NAME="savagewheels_1.4.tar.gz"
	
	cd $TMP_PATH
	$TAR -czf $PAK_NAME --exclude=.svn *
	#mv $PAK_NAME $TMP_PATH -f
	cd ..

	echo "Build done."
}

if [ "$1" = "clean" ]; then
	cleanup
elif [ "$1" = "build" ]; then
	cleanup
	build
else
	usage
fi
