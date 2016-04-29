# CMakeLists.txt for directory test

# see /usr/share/cmake-2.8/Modules/FindGTest.cmake
SET(GTEST_ROOT "/usr/src/gtest")

enable_testing()
FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

FIND_PACKAGE(ZeroMQ REQUIRED)

AUX_SOURCE_DIRECTORY(. TEST_SRCS)
ADD_EXECUTABLE(spl_unittest ${TEST_SRCS})

TARGET_LINK_LIBRARIES(spl_unittest spl;${GTEST_BOTH_LIBRARIES};pthread)
