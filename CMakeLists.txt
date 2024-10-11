cmake_minimum_required(VERSION 3.14)
project(zophyros)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories($ENV{HOME}/.local/include)
link_directories($ENV{HOME}/.local/lib)

add_library(zophyros httpServer.cc httpRoute.cc)

add_subdirectory(test)