#!/bin/bash

PARAMS=""

while (( "$#" )); do 
	case "$1" in
		--extra-lib-dir=*)
			export LD_LIBRARY_PATH="$(echo $1 | sed "s/--extra-lib-dir//"):$LD_LIBRARY_PATH" 
			shift
			;;
		-l)
			shift
			export LD_LIBRARY_PATH="$1:$LD_LIBRARY_PATH"
			shift
			;;
		*)
			PARAMS="$PARAMS $1"
			shift 
			;;
	esac 
done 

valve-interface-query64 $PARAMS
