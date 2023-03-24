#!/bin/bash

./autogen.sh
emconfigure ./configure
#emmake make EXEEXT=".html" CFLAGS="-O3  $EM_FLAGS" LDFLAGS="--shell-file em_shell.html"
#emmake make EXEEXT=".js" CFLAGS="-O3  $EM_FLAGS" LDFLAGS="--preload-file ../wasm_assets/"
emmake make EXEEXT=".js" CFLAGS="-O3  $EM_FLAGS" LDFLAGS="--preload-file ../wasm_assets/ -s MODULARIZE=1 -s FORCE_FILESYSTEM=1" 
mkdir -p build_wasm
cp src/kalign.* ./build_wasm/

