cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

project(ReactionSimulation)

# Find root-config and set the necessary flags
find_program(ROOT_CONFIG_EXEC root-config)
if(NOT ROOT_CONFIG_EXEC)
    message(FATAL_ERROR "Failed to find root-config. Make sure ROOT is correctly installed.")
endif()

# Include directories for PLUTO
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include /home/WASA-software/pluto/src
)
include_directories(
        ${PLUTOSYS}/lib
        ${PLUTOSYS}/plugins/brems
        ${PLUTOSYS}/plugins/dalitz_mod
        ${PLUTOSYS}/plugins/elementary
        ${PLUTOSYS}/plugins/eta_decays
        ${PLUTOSYS}/plugins/fairroot
        ${PLUTOSYS}/plugins/hades
        ${PLUTOSYS}/plugins/nucleus_fermi
        ${PLUTOSYS}/plugins/pdg_unigen
        ${PLUTOSYS}/plugins/pion_beam
        ${PLUTOSYS}/plugins/scatter_mod
        ${PLUTOSYS}/plugins/strangeness
        ${PLUTOSYS}/plugins/w_decays
    )

# Execute root-config to get compiler flags and libraries
execute_process(COMMAND ${ROOT_CONFIG_EXEC} --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${ROOT_CONFIG_EXEC} --libs OUTPUT_VARIABLE ROOT_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Define the executable and specify source files
add_executable(generator src/main.cpp src/ReactionSimulation.cpp)

# Link the executable with ROOT libraries and PLUTO library
target_link_libraries(generator ${ROOT_LIBRARIES} /home/WASA-software/pluto/libPluto.so)

# Set output directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# Additional compile options
add_compile_options(generator PRIVATE -Wall -Wextra -pedantic)

