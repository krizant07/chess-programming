#!/bin/sh
mkdir -p build
cd build
cmake ..
make
mv board ..
mv compile_commands.json ..
cd ..
