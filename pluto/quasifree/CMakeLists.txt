cmake_minimum_required(VERSION 3.0)
project(EventGenerator)

# Find ROOT
find_package(ROOT REQUIRED)

# Include directories
include_directories(${ROOT_INCLUDE_DIRS})

# Add your source files here
set(SOURCES
    src/main.cpp
    src/file_reader.cpp
    src/event_generator.cpp
    src/data_writer.cpp
)

# Define the executable that will be built from the source files
add_executable(run_simulate ${SOURCES})

# Link against ROOT libraries
target_link_libraries(run_simulate ${ROOT_LIBRARIES})
