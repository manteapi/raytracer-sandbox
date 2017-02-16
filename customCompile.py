import os
import sys

PREFIX_PATH="-DCMAKE_PREFIX_PATH=/opt/Qt/5.8/gcc_64/lib/cmake/Qt5"

if len(sys.argv)==2:
    if sys.argv[1]=="all":
        os.system("mkdir -p build && cd build && cmake "+PREFIX_PATH+" .. && make -j 8")
    elif sys.argv[1]=="clean":
        os.system("rm -r build/")
else :
    print("python compile.py all : compile all \n python compile.py clean : clean all")
