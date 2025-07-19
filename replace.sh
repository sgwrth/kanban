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

result=$(find "$dir" -type f | grep -E "$fileTypes" | xargs grep -E "$oldTerm")

find "$dir" -type f | grep -E "$fileTypes" | xargs sed -i "s/$oldTerm/$newTerm/g"

# Early return if no results were found.
if [[ "$result" == "" ]]; then
    printf "Replaced: 0\n"
    exit 0
fi

printf "$result\n" | sed "s/$oldTerm/$newTerm/g"
printf "$result" | awk 'END { print "Replaced:", NR }'
