cmake_minimum_required(VERSION 2.6)

project(ReactionGenerator)

# Ensure C++98 compatibility
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Execute root-config to get compiler flags and libraries
execute_process(COMMAND ${ROOT_CONFIG_EXEC} --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${ROOT_CONFIG_EXEC} --libs OUTPUT_VARIABLE ROOT_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

# Define the executable and specify source files
add_executable(pluto_run src/main.cpp src/reaction_generator.cpp)

# Link the executable with ROOT libraries and PLUTO library
target_link_libraries(pluto_run ${ROOT_LIBRARIES} $ENV{PLUTOSYS}/libPluto.so)

# Set output directory and additional compile options
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
# add_compile_options(-Wall -Wextra -pedantic)
