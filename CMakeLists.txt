cmake_minimum_required(VERSION 3.16)

project(Inferno VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Define the source file (The single large file as requested)
set(INFERNO_SOURCES
    inferno_full_code.cpp
)

# Use Qt's automatic MOC (Meta-Object Compiler) processing
# We use the single file as the source
qt_add_executable(Inferno
    ${INFERNO_SOURCES}
)

# Link the necessary Qt libraries
target_link_libraries(Inferno PRIVATE Qt6::Core Qt6::Widgets)

# Set the output directory for the executable
set_target_properties(Inferno PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Set application properties for Windows
if (WIN32)
    # Hide the console window on Windows
    set_target_properties(Inferno PROPERTIES WIN32_EXECUTABLE TRUE)
    # NOTE: To add a proper icon, a resource file (.rc) is needed.
    # This is a placeholder for a full build setup.
endif()
