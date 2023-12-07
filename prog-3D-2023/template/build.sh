#!/bin.sh

mkdir -p build
cd build
cmake ..
make -j 24
cd ..
