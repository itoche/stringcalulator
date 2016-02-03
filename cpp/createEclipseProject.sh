#!/bin/bash

rm -rf build
mkdir -p build
cd build
cmake -G "Eclipse CDT4 - Ninja" -DCMAKE_BUILD_TYPE=Debug  -DCMAKE_CXX_COMPILER_ARG1=-std=c++11  ../project
