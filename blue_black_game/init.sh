#!/bin/bash

dir_list="build build/src build/test"

for dir in $dir_list; do
    if [ ! -d "$dir" ]; then
        mkdir "$dir"
    fi
done

