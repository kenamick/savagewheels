#!/bin/bash
# Desc: Render bin/sound/*.it (Impulse Tracker) music to .ogg (Opus) for the
#       Emscripten/WebAssembly build. Browsers cannot decode .it modules, so
#       CSounds.cpp loads these .ogg counterparts instead when __EMSCRIPTEN__
#       is defined -- see the LOAD_MUSIC calls in src/CSounds.cpp.
#
# Requires:
#   openmpt123  (brew install libopenmpt)  -- renders tracker modules to PCM
#   ffmpeg      (brew install ffmpeg)      -- encodes PCM to Ogg/Opus
#
# Usage: tools/convert-music-web.sh

set -euo pipefail

CUR_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SOUND_DIR="$CUR_DIR/bin/sound"

command -v openmpt123 >/dev/null || { echo "openmpt123 not found. Install with: brew install libopenmpt"; exit 1; }
command -v ffmpeg >/dev/null || { echo "ffmpeg not found. Install with: brew install ffmpeg"; exit 1; }

shopt -s nullglob
IT_FILES=("$SOUND_DIR"/*.it)
if [ ${#IT_FILES[@]} -eq 0 ]; then
	echo "No .it files found in $SOUND_DIR -- did you extract the game data per INSTALL.md?"
	exit 1
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

for it_file in "${IT_FILES[@]}"; do
	name="$(basename "$it_file" .it)"
	wav_file="$TMP_DIR/$name.wav"
	ogg_file="$SOUND_DIR/$name.ogg"

	echo "Converting $name.it -> $name.ogg ..."
	openmpt123 --render --no-float --output-type wav --samplerate 44100 --channels 2 -q "$it_file"
	mv "$SOUND_DIR/$name.it.wav" "$wav_file"
	ffmpeg -y -loglevel error -i "$wav_file" -c:a libopus -b:a 96k "$ogg_file"
done

echo "Done. Converted ${#IT_FILES[@]} file(s) into $SOUND_DIR"
