cmake_minimum_required(VERSION 2.6)
project(raytracer-sandbox_test)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(../)

add_subdirectory(gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(
    ALL_LIBS
    gtest
    gtest_main
    )

add_custom_target(raytracer-sandbox_test
    COMMAND ./defaultTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Launch Raytracer-sandbox Tests" VERBATIM
)

add_executable(defaultTest defaultTest.cpp)
target_link_libraries(defaultTest ${ALL_LIBS})
add_test(DefaultTest defaultTest)
