#==========================================
#Project options
#==========================================
cmake_minimum_required(VERSION 2.8.11)

#==========================================
#Project name
#==========================================
project(raytracer-app)

#==========================================
#Building options
#==========================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall")

#==============
#Libraries path 
#==============

#Raytracer-sandbox
set(RAYTRACER_SANDBOX_INCLUDE_DIRS "" CACHE PATH "path to raytracer_sandbox headers")
include_directories(${RAYTRACER_SANDBOX_INCLUDE_DIRS})
set(RAYTRACER_SANDBOX_LIBRARIES "" CACHE FILEPATH "path to raytracer_sandbox library")

#OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#QT5
find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

#OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

#GLM Libraries
set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../raytracer-sandbox/extlib/glm-0.9.9.0/" CACHE PATH "glm")
include_directories(${GLM_INCLUDE_DIRS})

#TINYOBJLOADER Libraries
set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../raytracer-sandbox/extlib/tinyobjloader/" CACHE PATH "tinyobjloader")
include_directories(${TINYOBJLOADER_INCLUDE_DIRS})

include_directories(include/)

#==============================================
#Project sources
#==============================================
file(
    GLOB_RECURSE
    RAYTRACER_APP_HEADER
    include/*.hpp
    )

file(
    GLOB_RECURSE
    RAYTRACER_APP_SOURCE
    src/*.cpp
    )

file(
    GLOB
    QML_FILES
    qml/*.qml
    qml/components/*.qml
    )


file(
    GLOB
    JAVASCRIPT_FILES
    js/*.js
    )

file(
    GLOB
    QRC_FILES
    ressources/*.qrc
    )

file(
    GLOB
    SHADER_FILES
    shaders/*.glsl
    )

qt5_add_resources(RESOURCE_FILES ${QRC_FILES})

#==============================================
#Project executable
#==============================================
add_executable(raytracer-app ${RAYTRACER_APP_HEADER} ${RAYTRACER_APP_SOURCE} ${QRC_FILES} ${RESOURCE_FILES} ${QML_FILES} ${SHADER_FILES} ${JAVASCRIPT_FILES})
target_link_libraries(raytracer-app Qt5::Widgets Qt5::Qml Qt5::Quick ${OPENGL_LIBRARIES} ${RAYTRACER_SANDBOX_LIBRARIES})
