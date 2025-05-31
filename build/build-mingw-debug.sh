#!/bin/sh
cmake -DCMAKE_BUILD_TYPE=Debug .. \
      -G "MSYS Makefiles" \
      "$@" \
      ..

cmake --build .
