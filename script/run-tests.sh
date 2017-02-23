#!/bin/sh

#Release compilation and testing
cd raytracer-sandbox
mkdir -p buildRelease
cd buildRelease
cmake -DCMAKE_CXX_COMPILER=$COMPILER -DCMAKE_BUILD_TYPE=Release ..
make
make detailed_test
