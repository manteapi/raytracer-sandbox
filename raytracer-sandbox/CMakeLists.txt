#==========================================
#Project options
#==========================================
cmake_minimum_required(VERSION 2.8.11)

#==========================================
#Project name
#==========================================
project(RAYTRACER_SANDBOX)

#==========================================
#Building options
#==========================================
 # Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall")
#Needed by Qt - "You must build your code with position independent code if Qt was built with -reduce-relocations"
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if(CMAKE_BUILD_TYPE MATCHES Release)
    MESSAGE( STATUS "BUILD_TYPE=Release")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    MESSAGE( STATUS "BUILD_TYPE=Debug")
    #Required by gcov
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
    SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

#==============
#Libraries path 
#==============

#OpenMP
if(CMAKE_BUILD_TYPE MATCHES Release)
    find_package(OpenMP)
    if(OPENMP_FOUND)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif()
endif()

#GLM Libraries
set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/glm-0.9.9.0/" CACHE PATH "glm")
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
set(RAYTRACER_SANDBOX_INCLUDE_DIRS ${RAYTRACER_SANDBOX_SOURCE_DIR}/include)
set(RAYTRACER_SANDBOX_LIBRARIES RAYTRACER_SANDBOX)
MESSAGE( STATUS "Created variable RAYTRACER_SANDBOX_INCLUDE_DIRS:         " ${RAYTRACER_SANDBOX_INCLUDE_DIRS} )
#MESSAGE( STATUS "Created variable RAYTRACER_SANDBOX_LIBRARIES:         " ${RAYTRACER_SANDBOX_LIBRARIES} )

#==============================================
#Project test
#https://cmake.org/cmake/help/v3.5/module/FindGTest.html
#==============================================

#Compile GTest
add_subdirectory(test)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(
    GTEST_LIBS
    gtest
    gtest_main
    )

add_executable(defaultTest test/defaultTest.cpp)
target_link_libraries(defaultTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-DefaultTest defaultTest CONFIGURATIONS Debug)

add_executable(renderTest test/renderTest.cpp)
target_link_libraries(renderTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-RenderTest renderTest CONFIGURATIONS Debug)

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

