# cmake version to be used
cmake_minimum_required( VERSION 3.16 )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

project(printer)


add_library(
  printer
  STATIC
  betterprinter.cpp
  basicprinter.cpp)

target_include_directories(
  printer
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..)
