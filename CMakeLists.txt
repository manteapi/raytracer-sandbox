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
set(CMAKE_AUTOMOC ON)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall")

#==============
#Libraries path 
#==============

#OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#QT5
find_package(Qt5 COMPONENTS Core Widgets Qml Quick Core Network REQUIRED)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

#OpenGL
find_package(OpenGL REQUIRED)

#TINYOBJLOADER Libraries
set(TINYOBJLOADER_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/extlib/tinyobjloader/" CACHE PATH "tinyobjloader")
include_directories(${TINYOBJLOADER_INCLUDE_DIRS})

include_directories(headers/)

#==============================================
#Project sources
#==============================================
file(
    GLOB_RECURSE
    HEADER_FILES
    headers/*.hpp
    )

file(
    GLOB_RECURSE
    SOURCE_FILES
    sources/*.cpp
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
add_executable(raytracer-sandbox ${HEADER_FILES} ${SOURCE_FILES} ${QRC_FILES} ${RESOURCE_FILES} ${QML_FILES} ${SHADER_FILES} ${JAVASCRIPT_FILES})
target_link_libraries(raytracer-sandbox Qt5::Core Qt5::Widgets Qt5::Qml Qt5::Quick ${OPENGL_LIBRARIES} )

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

