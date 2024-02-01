#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "ReactionSimulation.h"
#include <TSystem.h>
#include <TROOT.h>

// Combine final product names and create a filename
void processArguments(int argc, char** argv,std::string& finalProducts,
                      std::string& filename) {
    for (int i = 1; i < argc; ++i) {
        finalProducts += argv[i];
        finalProducts += (i < argc - 1) ? " " : "";
        filename += argv[i];
    }
}

void printLoadedLibraries() {
    TString listOfLibraries = gSystem->GetLibraries();
    std::cout << "Loaded libraries: " << listOfLibraries.Data() 
              << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " product1 product2 ..."
                  << std::endl;
        return 1;
    }
    
    std::string finalProducts;
    std::string filename;
    
    processArguments(argc, argv, finalProducts, filename);
    
    // Initialize ROOT abd PLUTO
    const char* libraries[] = {
        "libMatrix.so", "libHist.so", "libPhysics.so",
        "libRIO.so", "libTree.so", "libTreeViewer.so",
        "${PLUTOSYS}/libPluto.so"
        };
    
    printLoadedLibraries();
    
    bool allLoaded = true;
    
    for (const char* library : libraries) {
        
        const char* libraryPath = gSystem->DynamicPathName(library, kTRUE);
        
        if (!libraryPath) {
            if (gSystem->Load(library)) {
                printf("Shared library %s loaded\n", library);
            } else {
                printf("Unable to load %s\n", library);
                allLoaded = false;
            }
        } else {
            printf("Library %s is already loaded at %s\n", 
                    library, libraryPath);
        }
    }
    
    if (allLoaded) {
        gSystem->SetIncludePath("-I${PLUTOSYS}/src");
        printf("All libraries loaded successfully.\n");
    } else {
        printf("Some libraries failed to load.\n");
    }
    
    // Perform reaction simulation
    ReactionSimulation react;
    
    for (int runindex = 0; runindex < 10; ++runindex) {
        std::cout << "Running simulation iteration "
                  << runindex + 1 << "/10..." << std::endl;
        react.simulate(finalProducts, filename, runindex + 1);
    }
    
    std::cout << "Simulation completed successfully." << std::endl;
    
    return 0;
}
