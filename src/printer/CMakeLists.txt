# cmake version to be used
cmake_minimum_required( VERSION 3.16 )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)


add_library(
  model
  STATIC
    betterprinter.cpp
    basicprinter.cpp)

target_include_directories(
  model
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/zero_rc)
