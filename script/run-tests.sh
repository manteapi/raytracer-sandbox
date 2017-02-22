#!/bin/sh

#Release compilation and testing
cd raytracer-sandbox
mkdir -p buildRelease
cd buildRelease
cmake -DCMAKE_BUILD_TYPE=Release ..
make
make test

#Back to root
cd ../../

#Release compilation and testing
cd raytracer-sandbox
mkdir -p buildDebug
cd buildDebug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
make test
