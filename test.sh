#!/usr/bin/bash
make clean
make
for file in input/*
do  
    bin/main.exe -c input/$(basename "$file") -o result/$(basename "$file").z78
    bin/main.exe -x result/$(basename "$file").z78 -o result/$(basename "$file")
    diff input/$(basename "$file") result/$(basename "$file") -q
done
la input/ result/*.z78