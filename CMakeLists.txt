CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_SUBDIRECTORY(googletest)

SET(CMAKE_CXX_STANDARD 11)

ADD_EXECUTABLE(test
    ./unit_tests/unit_test.cpp ./src/command.cpp ./src/and.cpp ./src/or.cpp ./src/semiColon.cpp ./src/input.cpp ./src/outputAppend.cpp ./src/outputOverride.cpp ./src/pipe.cpp
)

ADD_EXECUTABLE(rshell
    ./src/main.cpp ./src/command.cpp ./src/and.cpp ./src/or.cpp ./src/semiColon.cpp ./src/input.cpp ./src/outputAppend.cpp ./src/outputOverride.cpp ./src/pipe.cpp

)

TARGET_LINK_LIBRARIES(test gtest)
TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)
