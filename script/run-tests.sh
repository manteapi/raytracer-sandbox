#!/bin/sh

#Compile library
cd raytracer-sandbox
mkdir -p build
cd build
cmake ..
make

#Back to root
cd ../../

#Compile and execute tests
cd test
mkdir -p build
cd build
cmake -DRAYTRACER_SANDBOX_INCLUDE_DIRS=./../../raytracer-sandbox/include -DRAYTRACER_SANDBOX_LIBRARIES=./../../raytracer-sandbox/build/libRAYTRACER_SANDBOX.a ..
make
make raytracer-sandbox_test
