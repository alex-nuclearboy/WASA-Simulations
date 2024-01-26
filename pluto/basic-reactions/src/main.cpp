#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "ReactionSimulation.h"
#include <TSystem.h>
#include <TROOT.h>

// Combine final product names and create a filename
void processArguments(int argc, char** argv, 
					  std::string& finalProducts, 
					  std::string& filename) {
    for (int i = 1; i < argc; ++i) {
        finalProducts += argv[i];
        finalProducts += (i < argc - 1) ? " " : "";
        filename += argv[i];
    }
}

int main(int argc, char** argv) {
    // Check for correct number of arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " product1 product2 ..." 
				  << std::endl;
        return 1;
    }
    
    std::string finalProducts;
    std::string filename;
    
    processArguments(argc, argv, finalProducts, filename);
    
    // Initialize PLUTO
    TSystem ts;
    const char* libraries[] = {"libMatrix.so", "libHist.so", 
							   "libPhysics.so", "libRIO.so", 
							   "libTree.so", "libTreeViewer.so", 
                               "$PLUTOSYS/libPluto.so"};

    for (const auto& lib : libraries) {
		gSystem->Load(lib);
        if (gSystem->Load(lib) == -1) {
            std::cerr << "Failed to load " << lib << std::endl;
            return 1;
        }
    }

    gSystem->SetIncludePath("-I/$PLUTOSYS/src");
	
    // Perform reaction simulation
    ReactionSimulation react;
    
    for (int runindex = 1; runindex < 10; ++runindex) {
		std::cout << "Running simulation iteration " 
				  << runindex + 1 << "/10..." << std::endl;
		react.simulate(finalProducts, filename, runindex + 1);
    }
    
    std::cout << "Simulation completed successfully." << std::endl;

    return 0;
}
