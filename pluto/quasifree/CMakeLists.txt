cmake_minimum_required(VERSION 2.6)
project(QuasiFreeReactionGenerator)

# Ensure C++98 compatibility
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Find ROOT package
find_program(ROOT_CONFIG_EXEC root-config)
if(NOT ROOT_CONFIG_EXEC)
    message(FATAL_ERROR "Failed to find root-config. Make sure ROOT is correctly installed.")
endif()

# Include directories for PLUTO
if(NOT $ENV{PLUTOSYS} STREQUAL "")
    link_directories($ENV{PLUTOSYS})
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        $ENV{PLUTOSYS}/src
        $ENV{PLUTOSYS}/lib
        $ENV{PLUTOSYS}/plugins/brems
        $ENV{PLUTOSYS}/plugins/dalitz_mod
        $ENV{PLUTOSYS}/plugins/elementary
        $ENV{PLUTOSYS}/plugins/eta_decays
        $ENV{PLUTOSYS}/plugins/fairroot
        $ENV{PLUTOSYS}/plugins/hades
        $ENV{PLUTOSYS}/plugins/nucleus_fermi
        $ENV{PLUTOSYS}/plugins/pdg_unigen
        $ENV{PLUTOSYS}/plugins/pion_beam
        $ENV{PLUTOSYS}/plugins/scatter_mod
        $ENV{PLUTOSYS}/plugins/strangeness
        $ENV{PLUTOSYS}/plugins/w_decays
)
endif()

include_directories(${ROOT_INCLUDE_DIRS})

# Execute root-config to get compiler flags and libraries
execute_process(COMMAND ${ROOT_CONFIG_EXEC} --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${ROOT_CONFIG_EXEC} --libs OUTPUT_VARIABLE ROOT_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

# Add your source files here
set(SOURCES
    src/main.cpp
    src/file_reader.cpp
    src/physics_calculator.cpp
    src/event_generator.cpp
    src/data_writer.cpp
)

# Define the executable that will be built from the source files
add_executable(run_simulate ${SOURCES})

# Link the executable with ROOT and PLUTO libraries
target_link_libraries(run_simulate ${ROOT_LIBRARIES} $ENV{PLUTOSYS}/libPluto.so)
