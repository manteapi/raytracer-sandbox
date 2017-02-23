#!/bin/sh

#Release compilation and testing
cd raytracer-sandbox
mkdir -p buildRelease
cd buildRelease
cmake -DCMAKE_CXX_COMPILER=$COMPILER -DCMAKE_BUILD_TYPE=Release ..
make
make test

#Back to root
cd ../../

#Debug compilation, testing
cd raytracer-sandbox
mkdir -p buildDebug
cd buildDebug
cmake -DCMAKE_CXX_COMPILER=$COMPILER -DCMAKE_BUILD_TYPE=Debug ..
make
make test

#Back to root
cd ../../

#Updating coverage
cd script/
python gencoveragepng.py
