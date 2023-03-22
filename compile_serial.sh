#!/bin/bash

./autogen.sh
./configure --without-openmp
make
mkdir -p build_serial
cp src/kalign ./build_serial/

