#!/bin/sh
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="/usr" \
      -DRDSPARSER_DISABLE_TESTS=1 \
      "$@" \
      ..

cmake --build .
