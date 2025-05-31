#!/bin/sh
cmake -DCMAKE_BUILD_TYPE=Release \
      -G "MSYS Makefiles" \
      -DRDSPARSER_DISABLE_TESTS=1 \
      "$@" \
      ..

cmake --build .
