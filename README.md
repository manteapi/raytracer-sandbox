#raytracer-sandbox

[![Build Status](https://travis-ci.org/manteapi/raytracer-sandbox.svg?)](https://travis-ci.org/manteapi/raytracer-sandbox)

A c++ raytracer sandbox based on [Turner Whitted paper](http://dl.acm.org/citation.cfm?id=358882) and [ScratchAPixel](https://www.scratchapixel.com/) tutorials.

##Warning
This is a dummy raytracing implementation for learning purposes.
Currently it works fine for a sphere, plane, triangle meshes.
However it is very slow as no acceleration structures has been implemented yet.

##Organization
The raytracer-sandbox folder produces a library that implements our sandbox raytracer.
This library is tested in the test folder.
An example application using a GUI is proposed in the app folder.

##How to use it - Compilation, Executation and Cleaning

###Compile raytracer-sandbox library
    cd raytracer-sandbox
    mkdir build
    cmake ..
    make

###Test the library
    cd test
    python customCompile.py all
    make
    make raytracer-sandbox_test
    cd ..
    rm -r build

###Compile, execute and clean an application with a GUI

####Required libraries
To compile this project requires Qt5.8

####Instructions
    cd app
    #In the file customCompile.py, replace the -DCMAKE_PREFIX_PATH variable with your path to your Qt5.8 library.
    python customCompile.py all
    cd build
    ./raytracer-app

##Results

![First result](/results/firstResult.png?raw=true "First result")

##Todo

1. Use callgrind to compare performance between with/without broadphase collision.
2. Setup coverage tool.
3. Implement acceleration structure for triangular meshes.
