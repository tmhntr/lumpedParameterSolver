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


add_executable(
  ${PROJECT_NAME}
  ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/model
    ${CMAKE_SOURCE_DIR}/src/model/zero_rc
    ${CMAKE_SOURCE_DIR}/src/printer
    ${CMAKE_SOURCE_DIR}/src/solver
    ${SUNDIALS_INCLUDE_DIRS})

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    myLib)




# Make some test cases

add_executable(
  test
  ${CMAKE_SOURCE_DIR}/test/model/test_main.cpp)

target_include_directories(
  test
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(
  test
  PRIVATE
    component)
