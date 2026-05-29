#!/bin/sh
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
mv board ..
mv compile_commands.json ..
cd ..
