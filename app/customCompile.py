import os
import sys

COMPILE_FLAGS=""
PREFIX_PATH="-DCMAKE_PREFIX_PATH=/opt/Qt/5.8/gcc_64/lib/cmake/Qt5"
RAYTRACER_SANDBOX_INCLUDE_DIRS="-DRAYTRACER_SANDBOX_INCLUDE_DIRS=./../../raytracer-sandbox/include"
RAYTRACER_SANDBOX_LIBRARIES="-DRAYTRACER_SANDBOX_LIBRARIES=./../../raytracer-sandbox/buildRelease/libRAYTRACER_SANDBOX.a"

if len(sys.argv)==2:
    if sys.argv[1]=="all":
        os.system("mkdir -p build && cd build && cmake "+PREFIX_PATH+" "+COMPILE_FLAGS+" "+RAYTRACER_SANDBOX_INCLUDE_DIRS+" "+RAYTRACER_SANDBOX_LIBRARIES+" .. && make -j 8")
    elif sys.argv[1]=="clean":
        os.system("rm -r build/")
else :
    print("How to compile : \n \t python "+__file__+" all \n How to clean ? \n \t python "+__file__+" clean")
