#raytracer-sandbox

[![Build Status](https://travis-ci.org/manteapi/raytracer-sandbox.svg?)](https://travis-ci.org/manteapi/raytracer-sandbox)

A c++ raytracer sandbox based on [Turner Whitted paper](http://dl.acm.org/citation.cfm?id=358882) and [ScratchAPixel](https://www.scratchapixel.com/) tutorials.

##Warning
This is a dummy raytracing implementation for learning purposes.
Currently it works fine for a sphere, plane, triangle meshes.
However it is very slow as no acceleration structures has been implemented yet.

##Required Libraries
Qt5.8 is required. I use it to display the result at the end of the program.
Currently it is not really useful but I plan to make an extensive use of it in the future.

##Compilation, Execution and Cleaning

In the file customCompile.py, replace the -DCMAKE_PREFIX_PATH variable with your path to your Qt5.8 library.

To compile, type in a terminal the following command
    python customCompile.py all

To execute the program, move to the build directory which has been created during the compilation and execute the raytracer-sandbox executable.
    cd build
    ./raytracer-sandbox

To clean the build files, go back to the main directory and type the following command
    python customCopile.py clean

##Results

![First result](/results/firstResult.png?raw=true "First result")
