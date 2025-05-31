#!/bin/sh
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
      "$@" \
      ..

cmake --build .
