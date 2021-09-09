# cmake version to be used
cmake_minimum_required( VERSION 3.16 )

project(solver)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules")


find_package(
  SUNDIALS
  REQUIRED
  COMPONENTS
  sundials_cvode
  sundials_nvecserial)

add_library(
  solver
  STATIC
    sdsolver.cpp)

target_include_directories(
  solver
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${SUNDIALS_INCLUDE_DIRS})

target_link_libraries(
  solver
  PRIVATE
    SUNDIALS::sundials_cvode
    SUNDIALS::sundials_nvecserial)
