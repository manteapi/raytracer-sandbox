#==========================================
#Project options
#==========================================
cmake_minimum_required(VERSION 2.8)

#==========================================
#Project name
#==========================================
project(raytracer-sandbox)

#==========================================
#Building options
#==========================================
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall")
#Needed by Qt : "You must build your code with position independent code if Qt was built with -reduce-relocations"
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
#Required by gcov
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

#==============
#Libraries path 
#==============

#OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#GLM Libraries
set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/glm-0.9.7.1/" CACHE PATH "glm")
include_directories(${GLM_INCLUDE_DIRS})

#TINYOBJLOADER Libraries
set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/tinyobjloader/" CACHE PATH "tinyobjloader")
include_directories(${TINYOBJLOADER_INCLUDE_DIRS})

include_directories(include/)

#==============================================
#Project sources
#==============================================
file(
    GLOB_RECURSE
    RAYTRACER_SANDBOX_HEADER
    include/raytracer-sandbox/*.hpp
    )

file(
    GLOB_RECURSE
    RAYTRACER_SANDBOX_SOURCE
    src/*.cpp
    )

#==============================================
#Project library
#==============================================
add_library(RAYTRACER_SANDBOX
    ${RAYTRACER_SANDBOX_SOURCE}
    ${RAYTRACER_SANDBOX_HEADER}
)

#==============================================
#Doxygen command
#==============================================
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        file(MAKE_DIRECTORY doc) 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
endif(DOXYGEN_FOUND)

set(EXTRA_CLEANING
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/html
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/latex
    ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen_sqlite3.db
    )
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${EXTRA_CLEANING}")

