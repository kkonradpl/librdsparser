#!/bin/sh
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
      -DRDSPARSER_DISABLE_TESTS=1 \
      "$@" \
      ..

cmake --build .
