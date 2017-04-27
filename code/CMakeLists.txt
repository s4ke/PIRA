cmake_minimum_required (VERSION 3.0)
project(pira-code)

project (registerer)

##################################
# Download and install GoogleTest

include(ExternalProject)
ExternalProject_Add(gtest
        URL https://github.com/google/googletest/archive/release-1.8.0.zip

        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
        INSTALL_COMMAND "")
ExternalProject_Get_Property(gtest source_dir binary_dir)

################
# Define a test
#add_executable(registerer_test registerer_test.cc)

######################################
# Configure the test to use GoogleTest
#
# If used often, could be made a macro.

#add_dependencies(registerer_test gtest)
#include_directories(${source_dir}/include)
#target_link_libraries(registerer_test ${binary_dir}/libgtest.a)
#target_link_libraries(registerer_test ${binary_dir}/libgtest_main.a)

##################################
# Just make the test runnable with
#   $ make test

#enable_testing()
#add_test(NAME    registerer_test
#        COMMAND registerer_test)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/uebung1/main.cpp)
add_executable(pira-code ${SOURCE_FILES})


