# raytracer-sandbox

[![Build Status](https://travis-ci.org/manteapi/raytracer-sandbox.svg?branch=master)](https://travis-ci.org/manteapi/raytracer-sandbox)

![Coverage status](/.prettify-readme/coverage.png?raw=true "Coverage status")

A c++ raytracer sandbox based on [Turner Whitted paper](http://dl.acm.org/citation.cfm?id=358882) and [ScratchAPixel](https://www.scratchapixel.com/) tutorials.

## Warning
This is a dummy raytracing implementation for learning purposes.
Currently it works fine for a sphere, plane, triangle meshes.
However it is very slow as no acceleration structures has been implemented yet.

## Organization
The raytracer-sandbox folder produces a library that implements our sandbox raytracer.
This library is tested in the test folder.
An example application using a GUI is proposed in the app folder.

## How to use it - Compilation, Executation and Cleaning

### Compile raytracer-sandbox library
    cd raytracer-sandbox
    mkdir buildRelease
    cd buildRelease
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

### Test the library
    cd raytracer-sandbox
    mkdir buildDebug
    cd buildDebug
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make
    make test

### Compile, execute and clean an application with a GUI

#### Required libraries
To compile this project requires Qt5.8.

#### Instructions
First compile the library and then follow these instructions
    cd app
    #In the file customCompile.py, replace the -DCMAKE_PREFIX_PATH variable with your path to your Qt5.8 library.
    python customCompile.py all
    cd build
    ./raytracer-app

## Results

![First result](/app/results/firstResult.png?raw=true "First result")

## Roadmap
1. Implement an acceleration structure for triangular meshes.
2. Implement physically-based materials.
3. Perfom a robust library development : units test, coverage, performance
