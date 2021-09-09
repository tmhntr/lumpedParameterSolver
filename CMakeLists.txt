# cmake version to be used
cmake_minimum_required( VERSION 3.16 )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

# project name
project(rat)

# file(GLOB SOURCES "src/*.cpp")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} src src/model src/printer src/solver)

add_library(myLib)

target_sources(myLib
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src/model/RCR.cpp
    ${CMAKE_SOURCE_DIR}/src/model/component.cpp
    ${CMAKE_SOURCE_DIR}/src/model/heart.cpp
    ${CMAKE_SOURCE_DIR}/src/model/myoRC.cpp
    ${CMAKE_SOURCE_DIR}/src/model/myogenic.cpp
    ${CMAKE_SOURCE_DIR}/src/model/vpr_circulation.cpp
    ${CMAKE_SOURCE_DIR}/src/model/wrapper.cpp
    ${CMAKE_SOURCE_DIR}/src/model/zero_rc/zero_rc.cpp
    ${CMAKE_SOURCE_DIR}/src/printer/basicprinter.cpp
    ${CMAKE_SOURCE_DIR}/src/printer/betterprinter.cpp
    ${CMAKE_SOURCE_DIR}/src/solver/sdsolver.cpp)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(
  SUNDIALS
  REQUIRED
  COMPONENTS
    sundials_cvode
    sundials_nvecserial)

target_include_directories(
  myLib
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/model
    ${CMAKE_SOURCE_DIR}/src/model/zero_rc
    ${CMAKE_SOURCE_DIR}/src/printer
    ${CMAKE_SOURCE_DIR}/src/solver
  PRIVATE
    ${SUNDIALS_INCLUDE_DIRS})

target_link_libraries(
  myLib
  PRIVATE
    SUNDIALS::sundials_cvode
    SUNDIALS::sundials_nvecserial)

# add_subdirectory(${CMAKE_SOURCE_DIR}/src/model)
# add_subdirectory(${CMAKE_SOURCE_DIR}/src/printer)
# add_subdirectory(${CMAKE_SOURCE_DIR}/src/solver)

# find_library(MODEL NAMES model PATHS ${CMAKE_SOURCE_DIR}/src/model REQUIRED)
# find_library(PRINTER NAMES printer PATHS ${CMAKE_SOURCE_DIR}/src/printer REQUIRED)
# find_library(SOLVER NAMES solver PATHS ${CMAKE_SOURCE_DIR}/src/solver REQUIRED)

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
  testmodel
  ${CMAKE_SOURCE_DIR}/test/model/test_main.cpp)

target_include_directories(
  testmodel
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/model
    ${CMAKE_SOURCE_DIR}/src/model/zero_rc
    ${CMAKE_SOURCE_DIR}/src/printer
    ${CMAKE_SOURCE_DIR}/src/solver
    ${SUNDIALS_INCLUDE_DIRS})

target_link_libraries(
  testmodel
  PRIVATE
    myLib)
