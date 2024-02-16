#include "file_reader.h"
#include "event_generator.h"
#include "data_writer.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <TGraph.h>
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

    FileReader fileReader(model_file_path);
    TGraph* graph = fileReader.read_data();

    const int num_iterations = 10;
    for (int iteration = 1; iteration <= num_iterations; ++iteration) {       

        // Format output file name
        std::ostringstream file_name_stream;
        file_name_stream << "${PLUTO_OUTPUT}/pd-ppn_spec-" << model_name << "-" << iteration << ".root";
        std::string file_name = file_name_stream.str();

        EventGenerator eventGenerator(file_name);

        eventGenerator.setGraph(graph);
        eventGenerator.calculate();

        std::vector<std::pair<double, double> > calculated_data = eventGenerator.getCalculatedData();

        // Generate a text file name similar to the ROOT file name
        std::ostringstream text_file_name_stream;
        text_file_name_stream << "../proton_momentum_theta/proton_momentum_theta-" << model_name << "-" << iteration << ".txt";
        std::string text_file_name = text_file_name_stream.str();

        
        // Write results
        eventGenerator.generateEvents(1000);

        std::vector<std::pair<double, double> > collected_data = eventGenerator.getProtonData();


        DataWriter dataWriter;
        //dataWriter.writeSimulationData(tree, file_name);

       

        // Write the calculated data to a text file
        dataWriter.writeProtonData(collected_data, text_file_name);
    }

    // Cleanup
    delete graph;

    return 0;
}
