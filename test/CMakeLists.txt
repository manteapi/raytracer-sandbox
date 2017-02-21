#==========================================
#Project options
#==========================================
cmake_minimum_required(VERSION 2.8)

#==========================================
#Project name
#==========================================
project(raytracer-sandbox_test)

#==========================================
#Building options
#==========================================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#Required by gcov
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

#==============
#Libraries path 
#==============
#Raytracer-sandbox
set(RAYTRACER_SANDBOX_INCLUDE_DIRS "" CACHE PATH "path to raytracer_sandbox headers")
include_directories(${RAYTRACER_SANDBOX_INCLUDE_DIRS})
set(RAYTRACER_SANDBOX_LIBRARIES "" CACHE FILEPATH "path to raytracer_sandbox library")

find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#GLM Libraries
set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../raytracer-sandbox/extlib/glm-0.9.7.1/" CACHE PATH "glm")
include_directories(${GLM_INCLUDE_DIRS})

#TINYOBJLOADER Libraries
set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../raytracer-sandbox/extlib/tinyobjloader/" CACHE PATH "tinyobjloader")
include_directories(${TINYOBJLOADER_INCLUDE_DIRS})


#================
# Compile gtest
#================
add_subdirectory(gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(
    ALL_LIBS
    gtest
    gtest_main
    ${RAYTRACER_SANDBOX_LIBRARIES}
    )

add_custom_target(raytracer-sandbox_test
    COMMAND ./defaultTest
    COMMAND ./renderTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Launch Raytracer-sandbox Tests" VERBATIM
)

add_executable(defaultTest defaultTest.cpp ${RAYTRACER_APP_HEADER} ${RAYTRACER_APP_SOURCE})
target_link_libraries(defaultTest ${ALL_LIBS})

add_executable(renderTest renderTest.cpp ${RAYTRACER_APP_HEADER} ${RAYTRACER_APP_SOURCE})
target_link_libraries(renderTest ${ALL_LIBS})
