#!/bin/bash

dir=$1
class=$2
classUpper=$(printf "$class" | tr '[:lower:]' '[:upper:]')

touch "$dir"/"$class".h
touch "$dir"/"$class".cpp

printf "#ifndef "$classUpper"_H
#define "$classUpper"_H\n
class "$class" {\n
};\n
#endif" >> "$dir"/"$class".h

printf "#include \"./"$class".h\"\n\n" >> "$dir"/"$class".cpp
