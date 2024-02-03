#!/bin/bash
cmake -DCMAKE_BUILD_TYPE=Release \
      -DRDSPARSER_DISABLE_TESTS=1 \
      -G "MSYS Makefiles" ..

make
