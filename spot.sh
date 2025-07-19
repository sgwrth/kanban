#!/bin/bash

lang="$1"
dir="$2"
term="$3"

case "$1" in
	cpp)
		fileTypes=".h$|.cpp$"
		;;
	*)
		echo "Error: invalid language."
		exit 1
		;;
esac

result=$(find "$dir" -type f | grep -E "$fileTypes" | xargs grep -E "$term")

# Early return if no results were found.
if [[ "$result" == "" ]]; then
    printf "Found: 0\n"
    exit 0
fi

printf "$result\n"
printf "$result" | awk 'END { print "Found:", NR }'
