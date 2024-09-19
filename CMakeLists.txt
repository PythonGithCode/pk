# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and the version
project(MyCppProject VERSION 1.0)

# Set the output type to a shared library (DLL)
add_library(MyCppProject SHARED "D:/a/pk/pk/scr/mycode.cpp")

# Set the properties for generating the DLL
set_target_properties(MyCppProject PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release"
)


