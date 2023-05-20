#!/bin/bash

# build
rm -rf build
mkdir build
cd build
cmake ../src
make

# run
./mips_machine