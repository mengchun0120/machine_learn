#!/bin/bash

tests=$(ls build/test/)
for test in $tests; do
    build/test/$test
    if [ $? -ne 0 ]; then
        exit 1
    fi
done

exit 0

