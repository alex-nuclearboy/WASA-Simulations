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

#include "file_reader.h"
#include "event_generator.h"
#include "data_writer.h"
#include <iostream>
#include <string>
#include <sstream>
#include "TGraph.h"
#include "TSystem.h"

// Constants for the simulation
const int num_events = 1000;
const int num_iterations = 2;

/**
 * @brief Initialises and loads required libraries for the simulation.
 * 
 * This function attempts to load ROOT and PLUTO libraries needed for the 
 * simulation to run. It also sets the include path for PLUTO headers to ensure 
 * that the simulation can access necessary resources.
 * The function prints out all loaded libraries to provide feedback on the 
 * initialisation process.
 * 
 * @return true if all libraries are loaded successfully, false otherwise.
 *  * 
 * @note The function assumes that the PLUTOSYS environment variable is 
 *       correctly set to the path of the PLUTO installation.
 */
bool initialiseLibraries() {
    std::cout << "Initialising simulation environment..." << std::endl;

    // Attempt to load standard ROOT libraries.
    const char* common_libraries[] = {
        "libMatrix.so", "libHist.so", "libPhysics.so", "libRIO.so",
        "libTree.so", "libTreeViewer.so"
    };

    for (size_t i = 0; 
        i < sizeof(common_libraries) / sizeof(common_libraries[0]); 
        ++i) 
    {
        if (gSystem->Load(common_libraries[i]) == -1) {
            std::cerr << "Unable to load library: " << common_libraries[i] 
                      << std::endl;
            return false;
        }
    }

    // Construct the path for libPluto.so using the PLUTOSYS environment variable.
    std::string lib_pluto_path = std::string(getenv("PLUTOSYS")) + 
                                 "/lib/libPluto.so";

    // Attempt to load libPluto.so using the constructed path.
    if (gSystem->Load(lib_pluto_path.c_str()) == -1) {
        std::cerr << "Unable to load the PLUTO library" << std::endl;
        return false;
    }

    // Set the include path for PLUTO headers using the PLUTOSYS path.
    std::string include_path = std::string("-I") + getenv("PLUTOSYS") + "/src";
    gSystem->SetIncludePath(include_path.c_str());
    std::cout << "Include path set to: " << include_path << std::endl;

    std::cout << "All libraries loaded successfully." << std::endl;

    // Print the list of loaded libraries for confirmation.
    TString loaded_libraries = gSystem->GetLibraries();
    std::cout << "Loaded libraries:" << std::endl << loaded_libraries.Data() 
              << std::endl << std::endl;

    return true;
}


/**
 * @brief Manages the simulation runs for a given model and graph data.
 * 
 * This function is responsible for managing the execution of multiple 
 * simulation runs, generating event data, and writing the output to specified 
 * files for each simulation iteration. It uses the `EventGenerator`
 * class to process the simulations and the `DataWriter` class to handle the output.
 * 
 * @param graph A pointer to a TGraph object containing the nucleon momentum 
 *              distribution data for the model.
 * @param model_name The name of the model being simulated.
 */
void runSimulations(TGraph* graph, const std::string& model_name) {
    DataWriter dataWriter;

    for (int iteration = 0; iteration < num_iterations; ++iteration) {
        std::cout << "Processing simulation run " << (iteration + 1) << "..." 
                  << std::endl;

        std::string pluto_file_path = 
            DataWriter::getPlutoFilePath(model_name, iteration);
        std::string data_file_path = 
            DataWriter::getDataFilePath(model_name, iteration);
        std::string proton_file_path = 
            DataWriter::getProtonFilePath(model_name, iteration);
        
        // Initialise EventGenerator with the current model's graph and file names
        EventGenerator eventGenerator(graph, dataWriter, pluto_file_path,
                                      data_file_path, proton_file_path);
        
        // Generate and process events
        eventGenerator.generateEvents(num_events);

        std::cout << "Simulation run " << (iteration + 1) << " completed." 
                  << std::endl;
        std::cout << "PLUTO file: " << pluto_file_path << std::endl;
        std::cout << "Calculated data file: " << data_file_path << std::endl;
        std::cout << "Proton data file: " << proton_file_path 
                  << std::endl << std::endl;

    }
    std::cout << "Simulation completed successfully." << std::endl;
}

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

    if (!initialiseLibraries()) return 1;

    // Model selection and data loading
    std::string model_name = argv[1];
    TGraph* graph = FileReader::loadMomentumDistribution(model_name);
    if (graph == NULL || graph->GetN() <= 0) {
        std::cerr << "Error: Failed to load momentum distribution data." 
                  << std::endl;
        delete graph;
        return 1;
    }
        
    std::cout << "Initializing simulation for model: " << model_name 
              << std::endl << std::endl;

    runSimulations(graph, model_name);

    // Clean up
    delete graph;

    return 0;
}
