# ---------------------------------------------------------------
# Programmer(s): Timothy J. Hunter @ UWO
# ---------------------------------------------------------------
#

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add all of the shared MODEL components
add_subdirectory(component)
add_subdirectory(printer)
add_subdirectory(solver)
