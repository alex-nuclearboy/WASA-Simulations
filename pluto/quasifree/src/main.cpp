#include "file_reader.h"
#include "event_generator.h"
#include "data_writer.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "TGraph.h"
#include "TTree.h"
#include "TSystem.h"

void printLoadedLibraries() 
{
    TString libraries_list = gSystem->GetLibraries();
    std::cout << "Loaded libraries: " << libraries_list.Data() << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <Model Name>" << std::endl;
        return 1;
    }
    std::string model_name = argv[1];
    std::map<std::string, std::string> model_to_file_map;
    model_to_file_map["paris"] = "../momentum_distributions/paris_momentum_distribution.txt";
    model_to_file_map["cdbonn"] = "../momentum_distributions/cdbonn_momentum_distribution.txt";
    
    std::string model_file_path = model_to_file_map[model_name];

    if (model_file_path.empty()) {
        std::cerr << "Error: No file associated with model " << model_name << ". "
                  << "There are `paris` and `cdbonn` models available." << std::endl;
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

    FileReader file_reader(model_file_path);
    TGraph* graph = file_reader.read_data();

    DataWriter dataWriter;
  
    const int num_iterations = 10;
    for (int iteration = 1; iteration <= num_iterations; ++iteration) {     
        

       // Format the name of output files
        std::ostringstream pluto_file_name_stream;
        pluto_file_name_stream << "pd-ppn_spec-" << model_name << "-" << iteration << ".root";
        std::string pluto_file_name = pluto_file_name_stream.str();

        std::ostringstream data_file_name_stream;
        data_file_name_stream << "../data/data_ppn_spec-" << model_name << "-" << iteration << ".root";
        std::string data_file_name = data_file_name_stream.str();

        std::ostringstream proton_file_name_stream;
        proton_file_name_stream << "../data/proton_momentum_theta-" << model_name << "-" << iteration << ".txt";
        std::string proton_file_name = proton_file_name_stream.str();      
    
        EventGenerator eventGenerator(graph, dataWriter, pluto_file_name, data_file_name, proton_file_name);
        eventGenerator.generateEvents(1000);
    }

    // Cleanup
    delete graph;

    return 0;
}
