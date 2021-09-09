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

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} src src/model src/printer src/solver)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/model)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/printer)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/solver)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    model
    printer}
    solver)
