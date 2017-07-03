cmake_minimum_required (VERSION 3.0)
project(pira-code)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##################################
# Download and install GoogleTest

include(ExternalProject)
ExternalProject_Add(gtest
        URL https://github.com/google/googletest/archive/release-1.8.0.zip

        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
        INSTALL_COMMAND "")
ExternalProject_Get_Property(gtest source_dir binary_dir)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-std=c++11)

set(SOURCE_FILES src/uebung1/once.cpp)
add_executable(uebung1-1a ${SOURCE_FILES})

set(SOURCE_FILES src/uebung1/twice.cpp)
add_executable(uebung1-1b ${SOURCE_FILES})

set(SOURCE_FILES src/uebung1/threaded.cpp)
add_executable(uebung1-1c ${SOURCE_FILES})

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(PUBLIC uebung1-1c "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(uebung1-1c "${CMAKE_THREAD_LIBS_INIT}")
endif()

set(SOURCE_FILES src/uebung2/main.cpp)
add_executable(uebung2 ${SOURCE_FILES})

set(SOURCE_FILES src/uebung9/uebung9-1.cpp)
add_executable(uebung9 ${SOURCE_FILES})

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O0")
endif()



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



