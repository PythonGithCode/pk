# Minimum required version of CMake
cmake_minimum_required(VERSION 3.16)  # Updated to 3.16 based on the second file

# Set the project name and the version
project(MyCppProject VERSION 1.1 LANGUAGES CXX)

# If not specified, set the binary name
if(NOT BINARY_NAME)
    set(BINARY_NAME openttd)
endif()

# Require C++ standard
set(CMAKE_CXX_STANDARD 20)  # From second file
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Debug mode by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set the Python include directories and libraries
set(Python3_INCLUDE_DIRS "D:/a/pk/pk/include")
set(Python3_LIBRARIES "D:/a/pk/pk/libs/libpython3.11.dll.a")

# Locate Python
find_package(Python3 REQUIRED COMPONENTS Development)

# Set the output type to a shared library (DLL)
add_library(MyCppProject SHARED "D:/a/pk/pk/scr/mycode.cpp")

# Include Python directories and link libraries
target_include_directories(MyCppProject PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(MyCppProject PRIVATE ${Python3_LIBRARIES})

# Set properties for generating the DLL
set_target_properties(MyCppProject PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Release"
)

# Prevent in-source builds
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please run \"cmake ..\" from the build directory. You may need to delete \"${CMAKE_SOURCE_DIR}/CMakeCache.txt\" first.")
endif()

# Optionally, handle emscripten settings
if(EMSCRIPTEN)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/os/emscripten/cmake")
endif()

# Set CMake module paths
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Set deployment target for OSX
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

# Use GNUInstallDirs to allow customization of directories
include(GNUInstallDirs)

# Default data and bin directories
if(NOT CMAKE_INSTALL_DATADIR)
    set(CMAKE_INSTALL_DATADIR "share/games")
endif()
if(NOT CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR "games")
endif()

# Add subdirectories as needed
add_subdirectory(${CMAKE_SOURCE_DIR}/bin)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/media)

# Add custom targets for documentation, versioning, etc.
add_custom_target(find_version
        ${CMAKE_COMMAND}
                -DFIND_VERSION_BINARY_DIR=${CMAKE_BINARY_DIR}/generated
                -DCPACK_BINARY_DIR=${CMAKE_BINARY_DIR}
                -DREV_MAJOR=${PROJECT_VERSION_MAJOR}
                -DREV_MINOR=${PROJECT_VERSION_MINOR}
                -DWINDOWS=${WIN32}
                -P "${CMAKE_SOURCE_DIR}/cmake/scripts/FindVersion.cmake"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        BYPRODUCTS "${CMAKE_BINARY_DIR}/generated/rev.cpp"
)

# Documentation setup
if(DOXYGEN_EXECUTABLE)
    add_custom_target(docs)
    add_custom_target(docs_source
        ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating documentation for source"
    )
    add_dependencies(docs_source find_version)
    add_dependencies(docs docs_source)
endif()

# Include necessary directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/squirrel/include)

# Add custom target for tools if needed
add_custom_target(tools)

# Find necessary packages
find_package(Threads REQUIRED)
find_package(ZLIB)
find_package(LibLZMA)
find_package(LZO)
find_package(PNG)

# Handle specific platform-related libraries
if(APPLE OR UNIX)
    add_definitions(-DUNIX)
endif()

# Add the necessary dependencies for OpenGL, SDL2, etc.
if(NOT EMSCRIPTEN AND NOT OPTION_DEDICATED)
    link_package(Fluidsynth)
    link_package(SDL)
    link_package(SDL2 TARGET SDL2::SDL2)
    link_package(Allegro)
    link_package(FREETYPE TARGET Freetype::Freetype)
    link_package(Fontconfig TARGET Fontconfig::Fontconfig)
    link_package(Harfbuzz TARGET harfbuzz::harfbuzz)
    link_package(ICU_i18n)
    link_package(OpenGL TARGET OpenGL::GL)
endif()

# Specific platform requirements for Windows or other platforms
if(WIN32)
    target_link_libraries(MyCppProject PRIVATE ws2_32 winmm imm32 usp10 psapi winhttp bcrypt)
endif()

# Testing and regression
enable_testing()
add_subdirectory(regression)

# Set up the project for specific platform or debugging needs
if(APPLE OR WIN32)
    find_package(Pandoc)
endif()

# Ensure we handle installation and packaging correctly
include(InstallAndPackage)
