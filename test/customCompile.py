import os
import sys

RAYTRACER_SANDBOX_INCLUDE_DIRS="-DRAYTRACER_SANDBOX_INCLUDE_DIRS=./../../raytracer-sandbox/include"
RAYTRACER_SANDBOX_LIBRARIES="-DRAYTRACER_SANDBOX_LIBRARIES=./../../raytracer-sandbox/build/libRAYTRACER_SANDBOX.a"

if len(sys.argv)==2:
    if sys.argv[1]=="all":
        os.system("mkdir -p build && cd build && cmake "+" "+RAYTRACER_SANDBOX_INCLUDE_DIRS+" "+RAYTRACER_SANDBOX_LIBRARIES+" .. && make -j 8")
    elif sys.argv[1]=="clean":
        os.system("rm -r build/")
else :
    print("python compile.py all : compile all \n python compile.py clean : clean all")
