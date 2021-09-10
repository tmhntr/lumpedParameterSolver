# ---------------------------------------------------------------
# Programmer(s): Timothy J. Hunter @ UWO
# ---------------------------------------------------------------
#


# cmake version to be used
cmake_minimum_required( VERSION 3.16 )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

# project name
project(
  model
  VERSION 1.0.0
  LANGUAGES CXX)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Prohibit in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  print_error("In-source build prohibited.")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Add selected packages and modules to the build
add_subdirectory(src)



# Make some test cases

add_executable(
  test
  ${CMAKE_SOURCE_DIR}/test/model/test_main.cpp)

target_include_directories(
  test
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src/)

target_link_libraries(
  test
  PRIVATE
    component)
