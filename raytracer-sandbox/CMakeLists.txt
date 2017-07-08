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
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# C++14 is required
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall")
#Needed by Qt - "You must build your code with position independent code if Qt was built with -reduce-relocations"
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if(CMAKE_BUILD_TYPE MATCHES Release)
    MESSAGE( STATUS "BUILD_TYPE=Release")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND CMAKE_BUILD_TYPE MATCHES Debug)
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
    include/raytracer-sandbox/*.inl
    )

file(
    GLOB_RECURSE
    RAYTRACER_SANDBOX_SOURCE
    src/*.cpp
    )

#==============================================
#Project config file
#==============================================
configure_file(config.h.in ${CMAKE_BINARY_DIR}/generated/config.h)
include_directories( ${CMAKE_BINARY_DIR}/generated/)

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

add_executable(planeTest test/planeTest.cpp)
target_link_libraries(planeTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-PlaneTest planeTest CONFIGURATIONS Debug)

add_executable(sphereTest test/sphereTest.cpp)
target_link_libraries(sphereTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-SphereTest sphereTest CONFIGURATIONS Debug)

add_executable(tmeshTest test/tmeshTest.cpp)
target_link_libraries(tmeshTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-TmeshTest tmeshTest CONFIGURATIONS Debug)

add_executable(boxTest test/boxTest.cpp)
target_link_libraries(boxTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-BoxTest boxTest CONFIGURATIONS Debug)

add_executable(rayTest test/rayTest.cpp)
target_link_libraries(rayTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-RayTest rayTest CONFIGURATIONS Debug)

add_executable(utilsTest test/utilsTest.cpp)
target_link_libraries(utilsTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-UtilsTest utilsTest CONFIGURATIONS Debug)

add_executable(cameraTest test/cameraTest.cpp)
target_link_libraries(cameraTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-CameraTest cameraTest CONFIGURATIONS Debug)

add_executable(ioTest test/ioTest.cpp)
target_link_libraries(ioTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-ioTest ioTest CONFIGURATIONS Debug)

add_executable(materialTest test/materialTest.cpp)
target_link_libraries(materialTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-MaterialTest materialTest CONFIGURATIONS Debug)

add_executable(pointLightTest test/pointLightTest.cpp)
target_link_libraries(pointLightTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-PointLightTest pointLightTest CONFIGURATIONS Debug)

add_executable(directionalLightTest test/directionalLightTest.cpp)
target_link_libraries(directionalLightTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-DirectionalLightTest directionalLightTest CONFIGURATIONS Debug)

add_executable(spotLightTest test/spotLightTest.cpp)
target_link_libraries(spotLightTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-SpotLightTest spotLightTest CONFIGURATIONS Debug)

add_executable(renderTest test/renderTest.cpp)
target_link_libraries(renderTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-RenderTest renderTest CONFIGURATIONS Debug)

add_executable(pathtracingTest test/pathtracingTest.cpp)
target_link_libraries(pathtracingTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-PathtracingTest pathtracingTest CONFIGURATIONS Debug)

add_executable(extentTest test/extentTest.cpp)
target_link_libraries(extentTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-ExtentTest extentTest CONFIGURATIONS Debug)

add_executable(octreeTest test/octreeTest.cpp)
target_link_libraries(octreeTest ${GTEST_LIBS} ${RAYTRACER_SANDBOX_LIBRARIES})
add_test(RaytracerSandbox-OctreeTest octreeTest CONFIGURATIONS Debug)

#Test command with details
add_custom_target(detailed_test 
    COMMAND ./defaultTest
    COMMAND ./pathtracingTest
    COMMAND ./boxTest
    COMMAND ./tmeshTest
    COMMAND ./planeTest
    COMMAND ./sphereTest
    COMMAND ./rayTest
    COMMAND ./utilsTest
    COMMAND ./cameraTest
    COMMAND ./ioTest
    COMMAND ./materialTest
    COMMAND ./pointLightTest
    COMMAND ./spotLightTest
    COMMAND ./directionalLightTest
    COMMAND ./renderTest
    COMMAND ./octreeTest
    COMMAND ./extentTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Launch Detailed Test" VERBATIM
)

#==============================================
#Project coverage
#==============================================

if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND CMAKE_BUILD_TYPE MATCHES Debug)
    find_program(GCOV_EXECUTABLE gcov)
    find_program(LCOV_EXECUTABLE lcov)
    find_program(GENHTML_EXECUTABLE genhtml)

    if(NOT GCOV_EXECUTABLE)
        MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
    endif() # NOT GCOV_EXECUTABLE
    if(NOT LCOV_EXECUTABLE)
        MESSAGE(FATAL_ERROR "lcov not found! Aborting...")
    endif() # NOT LCOV_EXECUTABLE
    if(NOT GENHTML_EXECUTABLE)
        MESSAGE(FATAL_ERROR "genhtml not found! Aborting...")
    endif() # NOT GENHTML_EXECUTABLE


    #Setup target
    set(COVERAGE_DIR "${CMAKE_BINARY_DIR}/coverage")
    set(COVERAGE_REPORT_DIR "${COVERAGE_DIR}/report")
    SET(coverage_base "${COVERAGE_DIR}/raytracer-sandbox_base.info")
    SET(coverage_test "${COVERAGE_DIR}/raytracer-sandbox_test.info")
    SET(coverage_merge "${COVERAGE_DIR}/raytracer-sandbox_merge.info")
    SET(coverage_final "${COVERAGE_DIR}/raytracer-sandbox_final.info")
    set(RAYTRACER_SANDBOX_OBJECT_DIR "${CMAKE_BINARY_DIR}/CMakeFiles/RAYTRACER_SANDBOX.dir/src/")
    set(MAKE_TEST make test)

    set(LCOV_REMOVE_EXTRA "/usr/include/*" "/usr/lib/*" "${CMAKE_CURRENT_SOURCE_DIR}/extlib/*" "${CMAKE_CURRENT_SOURCE_DIR}/test/*")

    add_custom_target(coverage-gen
        # Create a directory for coverage data
        COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}

        # Clear counters
        COMMAND ${LCOV_EXECUTABLE} --zerocounters --directory ${RAYTRACER_SANDBOX_OBJECT_DIR}

        # Run initial/baseline lcov
        COMMAND ${LCOV_EXECUTABLE} --capture --initial --directory ${RAYTRACER_SANDBOX_OBJECT_DIR} --output-file ${coverage_base} 

        # Run tests
        COMMAND ${MAKE_TEST} 

        # Second run of lcov
        COMMAND ${LCOV_EXECUTABLE} --capture --directory ${RAYTRACER_SANDBOX_OBJECT_DIR} --output-file ${coverage_test} 

        # Combine lcov tracefiles before and after test
        COMMAND ${LCOV_EXECUTABLE} --add-tracefile ${coverage_base} --add-tracefile ${coverage_test} --output-file ${coverage_merge} 
        COMMAND ${LCOV_EXECUTABLE} --output-file ${coverage_final} --remove ${coverage_merge} ${LCOV_REMOVE_EXTRA} > ${COVERAGE_DIR}/coverageSummary.txt

        # Create a directory for the html report
        COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_REPORT_DIR}

        # Generate an html report about the coverage
        COMMAND ${GENHTML_EXECUTABLE} --prefix ${RAYTRACER_SANDBOX_SOURCE_DIR} --ignore-errors source ${coverage_final} --legend --title "Raytracer-sandbox commit" --output-directory=${COVERAGE_REPORT_DIR}

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Computing coverage based on the testsDoxygen" VERBATIM
        )
endif()


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

