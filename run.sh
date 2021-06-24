#!/bin/bash

RED="\033[1;31m"
GREEN="\e[1;32m"
NOCOLOR="\033[0m"

SRC="src/"
INCLUDE="./ -I/usr/include -O3 -I/usr/include/mlx -L/usr/include/mlx -lmlx -L/usr/lib -lXext -lX11 -lm "

SRC_FILE="${SRC}*.c"
FILES=""
for f in $SRC_FILE
do
	FILES="$FILES $f"
done
rm main

CMD="gcc  -o main $FILES -g -I $INCLUDE"
echo $CMD
$CMD

if [ $? -ne 0 ]; then
	echo -e "${RED}Compilation Fail${NOCOLOR}"
else
	echo -e "${GREEN}Compilation Finish${NOCOLOR}"
fi
