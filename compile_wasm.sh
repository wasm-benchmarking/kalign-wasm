#!/bin/bash

./autogen.sh
emconfigure ./configure
emmake make EXEEXT=".js" CFLAGS="-O3  $EM_FLAGS"
mkdir -p build_wasm
cp src/kalign.* ./build_wasm/

