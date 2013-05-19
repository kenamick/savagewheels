#!/bin/sh
# Desc: Savage Wheels Linux Build Script

CMD="$1"
CPU_CORES="2"
TAR="$(which tar)"
CUR_DIR="$(dirname $(readlink -f $0))"
BIN_PATH="$CUR_DIR/bin"
TMP_PATH="$CUR_DIR/build.tmp"
EXEC_PATH="$CUR_DIR/release"
CMAKE="$(which cmake)"
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

	cd $EXEC_PATH
	make clean
	cd ..
	
	rm $EXEC_PATH/* -rf

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
	
	cd $EXEC_PATH
	$CMAKE cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release
	make -j$CPU_CORES
	cd ..
	
	# copy executable
	
	if [ ! -e "$EXEC_PATH/savagewheels" ]; then
		echo "Executable was not found in $EXEC_PATH !"
		exit
	fi
	
	cp $EXEC_PATH/savagewheels $TMP_PATH
	
	# copy libs
	#cp $EXEC_PATH/*.so $TMP_PATH	
	cp $EXEC_PATH/libfmodex.so $TMP_PATH	
	cp $EXEC_PATH/libSDL.so $TMP_PATH	
	  
	# copy resources
	cp $BIN_PATH/* $TMP_PATH -R
	
	# copy misc files
	cp $CUR_DIR/LICENSE $TMP_PATH
	cp $CUR_DIR/HISTORY $TMP_PATH
	cp $CUR_DIR/run.sh $TMP_PATH
	chmod +x $TMP_PATH/run.sh
	
	# cleanup temp files
	rm $TMP_PATH/debug*.html -f
	rm $TMP_PATH/pref -f

	#echo "Creating archive ..."
	#PAK_NAME="savagewheels-$VERSION-linux.tar.gz"
	#cd $TMP_PATH
	#$TAR -czf $PAK_NAME --exclude=.svn *
	cd ..

	echo "Build done."
	#echo "You can find the $PAK_NAME archive in $TMP_PATH. Enjoy :)"
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
