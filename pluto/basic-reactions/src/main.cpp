#include <iostream>
#include <sstream>
#include <string>
#include "reaction_generator.h"
#include <TSystem.h>
#include <TROOT.h>

// Combine final product names and create a file name
void processArguments(int argc, char** argv,std::string& final_products,
                      std::string& file_name) 
{
    for (int i = 1; i < argc; ++i) {
        final_products += argv[i];
        final_products += (i < argc - 1) ? " " : "";
        file_name += argv[i];
    }
}

void printLoadedLibraries() 
{
    TString libraries_list = gSystem->GetLibraries();
    std::cout << "Loaded libraries: " << libraries_list.Data() << std::endl;
}

int main(int argc, char** argv) 
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " product1 product2 ..." 
                  << std::endl;
        return 1;
    }
    
    std::string final_products;
    std::string file_name;
    
    processArguments(argc, argv, final_products, file_name);
    
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
    
    // Perform reaction simulation
    ReactionGenerator gen;
    
    for (int runindex = 0; runindex < 10; ++runindex) {
        std::cout << "Running simulation iteration "
                  << runindex + 1 << "/10..." << std::endl;
        gen.simulate(final_products, file_name, runindex + 1);
    }
    
    std::cout << "Simulation completed successfully." << std::endl;
    
    return 0;
}
