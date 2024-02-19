/**
 * @file main.cpp
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Entry point for the simulation program.
 * 
 * @details
 * This program simulates quasi-elastic proton-deuteron scattering events 
 * using predefined models for the momentum distribution of nucleons within 
 * the deuteron. It initializes the necessary libraries, reads the nucleon 
 * momentum distribution from a file, generates scattering events, and writes 
 * the results to ROOT and text files.
 * 
 * Usage: ./simulation <Model Name>
 *        Model Name: The name of the model to use for nucleon momentum distribution. 
 *                    Supported models: 'paris', 'cdbonn'.
 * 
 * The program expects a model name as an input argument, which determines 
 * the momentum distribution to be used for the simulation.
 * Output files:
 *  - ROOT file containing the simulated events.
 *  - ROOT file with calculated data from the events.
 *  - Text file listing the proton momentum and scattering angle.
 * 
 * Ensure that ROOT and PLUTO libraries are correctly set up and accessible.
 * 
 * @version 1.1
 * @date 2024-02-15
 * @note Last updated on 2024-02-19
 * 
 * @remark Licensed under the GNU General Public License version 3.0 (GPLv3).
 */

#include "file_reader.h"
#include "event_generator.h"
#include "data_writer.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
#include <limits.h>
#include <unistd.h>
#include "TGraph.h"
#include "TSystem.h"

/**
 * @brief Prints the list of currently loaded ROOT and PLUTO libraries.
 */
void printLoadedLibraries() 
{
    TString libraries_list = gSystem->GetLibraries();
    std::cout << "Loaded libraries: " << libraries_list.Data() << std::endl;
}

/**
 * @brief Converts a relative file path to an absolute path.
 *
 * @param relative_path The relative file path to be converted.
 * @return The absolute path if successful, an empty string otherwise.
 * @throws std::cerr An error message if the absolute path cannot be resolved
 */
std::string getAbsolutePath(const std::string& relative_path) {
    char temp[PATH_MAX];
    char* resolved_path = realpath(relative_path.c_str(), temp);
    if (resolved_path) {
        return std::string(resolved_path);
    } else {
        std::cerr << "Error resolving absolute path for: " << relative_path 
                  << std::endl;
        return "";
    }
}

/**
 * @brief Main function.
 * 
 * Initializes the simulation by loading required ROOT and PLUTO libraries, 
 * reading nucleon momentum distribution data, generating events, and writing output.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return int Program exit status.
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <Model Name>" << std::endl;
        return 1;
    }
    std::string model_name = argv[1];

    // Map model names to their corresponding momentum distribution files
    std::map<std::string, std::string> model_to_file_map;
    model_to_file_map["paris"] = 
        "../momentum_distributions/paris_momentum_distribution.txt";
    model_to_file_map["cdbonn"] = 
        "../momentum_distributions/cdbonn_momentum_distribution.txt";
    
    std::string model_file_path = model_to_file_map[model_name];

    // Check if the model name is valid
    if (model_file_path.empty()) {
        std::cerr << "Error: No file associated with model " << model_name << ". "
                  << "Available models are `paris` and `cdbonn`." << std::endl;
        return 1;
    }

    char* pluto_sys_env = getenv("PLUTOSYS");
    if (!pluto_sys_env) {
        std::cerr << "Error: PLUTOSYS environment variable is not set." 
                  << std::endl;
        return 1;
    }

    // Initialize ROOT and PLUTO libraries
    const char* libraries[] = {
        "libMatrix.so", "libHist.so", "libPhysics.so", "libRIO.so", 
        "libTree.so", "libTreeViewer.so", "${PLUTOSYS}/libPluto.so"
        };
    
    bool all_loaded = true;
    
    for (int i = 0; i < sizeof(libraries)/sizeof(libraries[0]); ++i) {
        const char* library = libraries[i];
        if (!gSystem->DynamicPathName(library, kTRUE)) {
            if (gSystem->Load(library) == -1) {
                std::cerr << "Unable to load " << library << std::endl;
                all_loaded = false;
            } else {
                std::cout << "Shared library " << library << " loaded" 
                          << std::endl;
            }
        }
    }
    
    if (all_loaded) {
        gSystem->SetIncludePath("-I${PLUTOSYS}/src");
        std::cerr << "All libraries loaded successfully." << std::endl;
    } else {
        std::cerr << "Some libraries failed to load." << std::endl;
    }

    printLoadedLibraries();

    const int num_events = 1000;

    // Read nucleon momentum distribution data for the selected model
    FileReader file_reader(model_file_path);
    TGraph* graph = file_reader.read_data();

    if (graph == NULL || graph->GetN() <= 0) {
        std::cerr << "Error: Graph is empty or invalid." << std::endl;
        delete graph;
        return 1;
    }

    // Get PLUTO_OUTPUT environment variable
    const char* pluto_output_env = std::getenv("PLUTO_OUTPUT");
    if (pluto_output_env == NULL) {
        std::cerr << "Error: PLUTO_OUTPUT environment variable is not set." 
                  << std::endl;
        return 1;
    }
    std::string pluto_output_path = pluto_output_env;

    DataWriter dataWriter;

    std::cout << "Initializing simulation for model: " << model_name 
              << std::endl << std::endl;

    // Iterate through simulations, generating events and writing output files  
    const int num_iterations = 10;
    for (int iteration = 0; iteration < num_iterations; ++iteration) {
        std::cout << "Processing simulation run " << iteration + 1 << " of " 
                  << num_iterations << std::endl;

        // Format output file names
        std::ostringstream pluto_file_name_stream;
        std::ostringstream data_file_name_stream;
        std::ostringstream proton_file_name_stream;
        pluto_file_name_stream << pluto_output_path << "/pd-ppn_spec-" 
                               << model_name << "-" << (iteration + 1) << ".root";        
        data_file_name_stream << "../data/data_ppn_spec-" << model_name
                              << "-" << (iteration + 1) << ".root";
        proton_file_name_stream << "../data/proton_momentum_theta-" << model_name
                                << "-" << (iteration + 1) << ".txt";

        // Convert relative paths to absolute paths
        std::string pluto_file_path_abs = 
            getAbsolutePath(pluto_file_name_stream.str());

        std::string data_file_path_abs = 
            getAbsolutePath(data_file_name_stream.str());

        std::string proton_file_path_abs = 
            getAbsolutePath(proton_file_name_stream.str());
        
        // Initialize EventGenerator with the current model's graph and file names
        EventGenerator eventGenerator(graph, dataWriter, pluto_file_path_abs,
                                      data_file_path_abs, proton_file_path_abs);
        
        // Generate and process events
        eventGenerator.generateEvents(num_events);

        std::cout << "Simulation run " << (iteration + 1) << " completed." 
                  << std::endl;
        std::cout << "PLUTO file: " << pluto_file_path_abs << std::endl;
        std::cout << "Calculated data file: " << data_file_path_abs << std::endl;
        std::cout << "Proton data file: " << proton_file_path_abs 
                  << std::endl << std::endl;
    }

    std::cout << "Simulation completed successfully." << std::endl;

    // Clean up graph resource
    delete graph;

    return 0;
}
