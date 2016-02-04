#!/bin/bash
set -e
cp -r /repository /build
cd /build
mkdir build
cd build
cmake -G "Ninja" /build/cpp/project
ninja
ninja coverage
cp -r coverage /artifacts/cpp_coverage
