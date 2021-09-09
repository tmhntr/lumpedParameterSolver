# cmake version to be used
cmake_minimum_required( VERSION 3.16 )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)


add_library(
  model
  STATIC
  RCR.cpp
  component.cpp
  heart.cpp
  myoRC.cpp
  myogenic.cpp
  vpr_circulation.cpp
  zero_rc/zero_rc.cpp)

target_include_directories(
  model
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/zero_rc)
