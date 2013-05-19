#!/bin/bash
# Desc: Savage Wheels Linux Startup Script

CUR_DIR="$(dirname $(readlink -f $0))"

export LD_LIBRARY_PATH="$CUR_DIR":.

# Run the game
./savagewheels
