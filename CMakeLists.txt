# cmake version to be used
cmake_minimum_required( VERSION 3.0 )

# project name
project( rat )

file(GLOB SOURCES "src/*.cpp")
add_executable( ${PROJECT_NAME} ${SOURCES} )

# flags

# files

# include
include_directories( ./include ./src /usr/local/Cellar/sundials/5.7.0_1/include )

# target


# external libs
add_subdirectory( /usr/local/Cellar/sundials/5.7.0_1/lib libs/sundials )
target_link_libraries( rat cvode )
target_link_libraries( rat sunlinsoldense )

# add_subdirectory( ./libs/Randomize )
# target_link_libraries( binary randomize )

# add_library(randomize STATIC IMPORTED)
# set_property(TARGET randomize PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/Randomize/librandomize.a)
# target_link_libraries( binary randomize )
