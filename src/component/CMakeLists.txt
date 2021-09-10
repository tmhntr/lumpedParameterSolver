# ---------------------------------------------------------------
# Programmer(s): Timothy J. Hunter @ UWO
# ---------------------------------------------------------------
#

# cmake version to be used
cmake_minimum_required( VERSION 3.16 )


# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

install(CODE "MESSAGE(\"\nInstall components\n\")")

# Add variable component_SOURCES with the sources for the component library
set(component_SOURCES
  RCR.cpp
  component_model.cpp
  heart.cpp
  myoRC.cpp
  myogenic.cpp
  vpr_circulation.cpp
  zero_rc/zero_rc.cpp
  )

# Add variable component_HEADERS with the exported component header files
set(component_HEADERS
  RCR.hpp
  component_model.hpp
  heart.hpp
  myoRC.hpp
  myogenic.hpp
  vpr_circulation.hpp
  zero_rc/zero_rc.hpp
  )

add_library(
  component
  STATIC
    ${component_SOURCES}
    ${component_HEADERS})

target_include_directories(
  component
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/include
  )


# Add prefix with complete path to the component header files
# add_prefix(${SUNDIALS_SOURCE_DIR}/include/component/ component_HEADERS)


# Finished component
message(STATUS "Added component module")
