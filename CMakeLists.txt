# cmake version to be used
cmake_minimum_required( VERSION 3.16 )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

# project name
project(rat)

file(GLOB SOURCES "src/*.cpp")

add_executable(
  ${PROJECT_NAME}
  ${SOURCES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(
  SUNDIALS
  REQUIRED
  COMPONENTS
  sundials_cvode
  sundials_nvecserial)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
    lib/zero_rc
    )



# message(WARNING ${SUNDIALS_INCLUDE_DIRS})

add_library(
  zero_rc
  lib/zero_rc/zero_rc.cpp)


target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    zero_rc
    SUNDIALS::sundials_cvode
    SUNDIALS::sundials_nvecserial)
