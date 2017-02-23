#!/bin/sh


#If COMPILER is unset or set to empty string
#Then we are certainly running this script from home
#Otherwise it is certainly running through Travis CI
#In the first case the debug case is sufficient
#In the second case there is no need to update coverage
if [ -z "${COMPILER}" ]; then
    #Debug compilation, testing
    cd raytracer-sandbox
    mkdir -p buildDebug
    cd buildDebug
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make
    make test
    
    #Back to root
    cd ../../
    
    #Updating coverage
    cd script/
    python gencoveragepng.py
fi

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
