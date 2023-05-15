#!/bin/bash

make clean

test_files=("leeds.map" "test.map")

# for each
for test_file in "${test_files[@]}"
do
    make

    ./build/main $test_file

    make clean
done