# cmake version to be used
cmake_minimum_required( VERSION 3.0 )

set(CMAKE_CXX_STANDARD 11)


# project name
project(rat)

list(APPEND CMAKE_PREFIX_PATH /usr/local/Cellar/sundials/5.7.0_1 )
# find_library(cvode)
# find_library(sunlinsol)

find_library(
  cvode
  <var>
  NAMES
  cvode
  sundials_cvode
  PATHS
  /usr/local/Cellar/sundials/5.7.0_1
  REQUIRED)
  add_library(cvode SHARED IMPORTED GLOBAL)
  set_property(TARGET cvode PROPERTY IMPORTED_LOCATION /usr/local/Cellar/sundials/5.7.0_1/lib/libsundials_cvode.dylib)


  target_include_directories(cvode INTERFACE /usr/local/Cellar/sundials/5.7.0_1)



add_library(zero_rc lib/zero_rc/zero_rc.cpp)


file(GLOB SOURCES "src/*.cpp")
add_executable( ${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} cvode zero_rc)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  lib/zero_rc)
