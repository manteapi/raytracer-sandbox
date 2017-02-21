#!/bin/sh

cd test
mkdir build
cd build
cmake ..
make
make raytracer-sandbox_test
