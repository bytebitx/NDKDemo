cmake_minimum_required(VERSION 3.22.1)

set(NAME "Add")
set(PROJECT_NAME ${NAME})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SRCS})