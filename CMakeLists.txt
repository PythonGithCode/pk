# Minimum required version of CMake
cmake_minimum_required(VERSION 3.15)


# require c++ standerd
set(CMAKE_CXX_STANDARD 11)

set(Python3_INCLUDE_DIRS "D:/a/pk/pk/include")
set(Python3_LIBRARIES "D:/a/pk/pk/libs/libpython3.11.dll.a")

# Locate Python
find_package(Python3 REQUIRED COMPONENTS Development)

# Set the project name and the version
project(MyCppProject VERSION 1.1)


# Set the output type to a shared library (DLL)
add_library(MyCppProject SHARED "D:/a/pk/pk/scr/mycode.cpp")

target_include_directories(MyCppProject PRIVATE ${Python3_INCLUDE_DIRS})

target_link_libraries(MyCppProject PRIVATE ${Python3_LIBRARIES})

# Set the properties for generating the DLL
set_target_properties(MyCppProject PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release"
)


