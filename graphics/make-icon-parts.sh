#!/bin/bash
for size in 128 64 48 32 24 16; do
    inkscape --export-area=0:64:192:256 -w $size -h $size \
        --export-png=icon$size.png icon.svg
done
