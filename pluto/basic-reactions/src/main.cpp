#include <iostream>
#include <sstream>
#include "ReactionSimulation.h"
#include <TSystem.h>

int main(int argc, char** argv) {
    // Check for correct number of arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " product1 product2 ..." 
			<< std::endl;
        return 1;
    }

    // Combine final product names and create a filename
    std::string finalProducts;
    std::string filename;
    for (int i = 1; i < argc; ++i) {
        finalProducts += argv[i];
        finalProducts += (i < argc - 1) ? " " : "";
        filename += argv[i];
    }

    // Initialize ROOT
    TSystem ts;
    gSystem->Load("libMatrix.so");
    gSystem->Load("libHist.so");
    gSystem->Load("libPhysics.so");
    gSystem->Load("libRIO.so");
    gSystem->Load("libTree.so");
    gSystem->Load("libTreeViewer.so");
    gSystem->Load("$PLUTOSYS/libPluto.so");
    gSystem->SetIncludePath("-I/home/WASA-software/pluto/src");

    // Perform reaction simulation
    ReactionSimulation react;
    for (int runindex = 0; runindex < 10; ++runindex) {
        react.simulate(finalProducts, filename, runindex + 1);
    }

    return 0;
}
