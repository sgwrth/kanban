#!/bin/bash

lang="$1"
dir="$2"
oldTerm="$3"
newTerm="$4"

case "$1" in
	cpp)
		fileTypes=".h$|.cpp$"
		;;
	*)
		echo "Error: invalid language."
		exit 1
		;;
esac

find "$dir" -type f | grep -E "$fileTypes" | xargs grep -E "$oldTerm"
find "$dir" -type f | grep -E "$fileTypes" | xargs sed -i "s/$oldTerm/$newTerm/g"
