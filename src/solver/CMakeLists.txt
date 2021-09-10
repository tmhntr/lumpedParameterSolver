# ---------------------------------------------------------------
# Programmer(s): Timothy J. Hunter @ UWO
# ---------------------------------------------------------------
#

# cmake version to be used
cmake_minimum_required( VERSION 3.16 )


# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

install(CODE "MESSAGE(\"\nInstall solvers\n\")")


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Modules")

find_package(
  SUNDIALS
  REQUIRED
  COMPONENTS
  sundials_cvode
  sundials_nvecserial)


# Add variable component_SOURCES with the sources for the component library
set(solver_SOURCES
  sdsolver.cpp
  )

# Add variable component_HEADERS with the exported component header files
set(solver_HEADERS
  sdsolver.hpp
  )

add_library(
  solver
  STATIC
    ${solver_SOURCES}
    ${solver_HEADERS})

target_include_directories(
  solver
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/include
  )


target_link_libraries(
  solver
  PRIVATE
    SUNDIALS::sundials_cvode
    SUNDIALS::sundials_nvecserial)


# Finished component
message(STATUS "Added solver module")
