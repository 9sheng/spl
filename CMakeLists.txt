# CMakeLists.txt for the whole project

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
PROJECT(spl)

# Uncomment next line to debug the makefiles
#SET(CMAKE_VERBOSE_MAKEFILE TRUE)

# Custom cmake modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

SET(CMAKE_C_FLAGS   "-g -Wall -Wextra")
SET(CMAKE_CXX_FLAGS "-g -Wall -Wextra -std=c++0x")

# include directories
INCLUDE_DIRECTORIES(include)

# lib directories
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

