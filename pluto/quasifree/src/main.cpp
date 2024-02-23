/**
 * @file main.cpp
 * @brief Entry point for a program simulating quasi-elastic proton-deuteron 
 *        scattering events.
 *
 * @details 
 * This program simulates quasi-elastic proton-deuteron scattering events 
 * using predefined models for the momentum distribution of nucleons within 
 * the deuteron. It initialises the necessary libraries, reads the nucleon 
 * momentum distribution from a file, generates scattering events, and writes 
 * the results to ROOT and text files.
 * 
 * Required environment variables:
 * - ROOTSYS: Specifies the root installation directory.
 * - PLUTOSYS: Specifies the PLUTO simulation framework installation directory.
 * 
 * The program outputs:
 * - A ROOT file containing the simulated events.
 * - A ROOT file with calculated data from the events.
 * - A text file listing the proton momentum and scattering angle.
 * 
 * @version 2.0
 * @date 2024-02-15
 * @note Last updated on 2024-02-20.
 * 
 * @remark Licensed under the GNU General Public License version 3.0 (GPLv3).
 */

#include "momentum_data_loader.h"
#include "library_manager.h"
#include "event_generator.h"
#include "data_writer.h"
#include <iostream>
#include <string>
#include <sstream>
#include "TGraph.h"


// Constants for the simulation
const int NUM_EVENTS = 1000;
const int NUM_ITERATIONS = 2;

/**
 * @brief Main function to initialise the simulation for a specific model.
 * 
 * The program performs the following steps:
 * 1. Validates the command-line arguments and environment variables.
 * 2. Initialises the ROOT and PLUTO libraries required for the simulation.
 * 3. Reads the nucleon momentum distribution data for the specified model.
 * 4. Runs the simulation for a predefined number of iterations, generating 
 *    and processing events.
 * 5. Cleans up resources and exits.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 upon successful completion, or 1 if an error occurs.
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <Model Name>" << std::endl;
        return 1;
    }

    // Environment variable checks
    const char* root_sys_env = getenv("ROOTSYS");
    if (!root_sys_env) {
        std::cerr << "Error: ROOTSYS environment variable is not set." 
                  << std::endl;
        return 1;
    }

    const char* pluto_sys_env = getenv("PLUTOSYS");
    if (!pluto_sys_env) {
        std::cerr << "Error: PLUTOSYS environment variable is not set." 
                  << std::endl;
        return 1;
    }

    if (!LibraryManager::initialiseLibraries()) return 1;

    // Model selection and data loading
    std::string model_name = argv[1];
    TGraph* graph = MomentumDataLoader::loadDeuteronNMD(model_name);
    if (graph == NULL || graph->GetN() <= 0) {
        std::cerr << "Error: Failed to load momentum distribution data." 
                  << std::endl;
        delete graph;
        return 1;
    }

    std::cout << "Initializing simulation for model: " << model_name 
              << std::endl << std::endl;

    EventGenerator::runSimulations(model_name, graph, NUM_EVENTS, NUM_ITERATIONS);

    // Clean up
    delete graph;

    return 0;
}
