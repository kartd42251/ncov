project(train)
SET(CMAKE_CXX_FLAGS " -g -Wall -std=c++11 ")

set(SRC_LIST src/main.cpp)

add_executable(main ${SRC_LIST})

