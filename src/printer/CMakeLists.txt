# ---------------------------------------------------------------
# Programmer(s): Timothy J. Hunter @ UWO
# ---------------------------------------------------------------
#

# cmake version to be used
cmake_minimum_required( VERSION 3.16 )


# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

install(CODE "MESSAGE(\"\nInstall printers\n\")")

# Add variable component_SOURCES with the sources for the component library
set(printer_SOURCES
  basicprinter.cpp
  betterprinter.cpp
  )

# Add variable component_HEADERS with the exported component header files
set(printer_HEADERS
  basicprinter.hpp
  betterprinter.hpp
  )

add_library(
  printer
  STATIC
    ${printer_SOURCES}
    ${printer_HEADERS})

target_include_directories(
  printer
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/include
  )


# Finished component
message(STATUS "Added printer module")
