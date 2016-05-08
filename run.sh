#!/bin/bash
# Desc: Savage Wheels Linux Startup Script

CUR_DIR="$(dirname $(readlink -f $0))"

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CUR_DIR:.
export SAVAGEWHEELS_SYS_DATADIR="$CUR_DIR/share"
export SAVAGEWHEELS_LIBEXECDIR="$CUR_DIR/libexec"

# Run the game
exec bin/savagewheels "$@"
