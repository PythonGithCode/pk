# Add subdirectory for 'api' if it exists
if(EXISTS "${CMAKE_SOURCE_DIR}/src/script/api")
    add_subdirectory(api)
else()
    message(WARNING "Directory 'api' not found, skipping 'add_subdirectory'.")
endif()

# Check if OPTION_TOOLS_ONLY is enabled
if(OPTION_TOOLS_ONLY)
    return()
endif()

# Define the target for your project, assuming it is 'MyCppProject'
# Make sure that 'MyCppProject' is the name of your target (e.g., an executable or library)
target_sources(MyCppProject PRIVATE
    script_cmd.h
    script_config.cpp
    script_config.hpp
    script_fatalerror.hpp
    script_gui.h
    script_gui.cpp
    script_info.cpp
    script_info.hpp
    script_info_dummy.cpp
    script_instance.cpp
    script_instance.hpp
    script_scanner.cpp
    script_scanner.hpp
    script_storage.hpp
    script_suspend.hpp
    squirrel.cpp
    squirrel.hpp
    squirrel_class.hpp
    squirrel_helper.hpp
    squirrel_helper_type.hpp
    squirrel_std.cpp
    squirrel_std.hpp
)
