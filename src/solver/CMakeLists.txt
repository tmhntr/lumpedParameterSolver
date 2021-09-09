# cmake version to be used
cmake_minimum_required( VERSION 3.16 )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

# project name
project(solver)

add_library(solver STATIC sdsolver.cpp)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(
  SUNDIALS
  REQUIRED
  COMPONENTS
  sundials_cvode
  sundials_nvecserial)

target_include_directories(
  solver
  PRIVATE
    lib/zero_rc
    )

target_link_libraries(
  solver
  PRIVATE
    zero_rc
    SUNDIALS::sundials_cvode
    SUNDIALS::sundials_nvecserial)

target_include_directories(model
  PUBLIC
    $<INSTALL_INTERFACE:include/model>  # <prefix>/include/printer
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/model>
    ${CMAKE_SOURCE_DIR}/src/solver
    ${SUNDIALS_INCLUDE_DIRS})
