#include "ReactionSimulation.h"
#include <PBeamSmearing.h>
#include <PReaction.h>
#include <TF1.h>
#include <cstring>
#include <iostream>

void ReactionSimulation::simulate(const std::string& finalProducts,
                                  const std::string& filename,
                                  int iter) {

    // Create beam smearing and set parameters
    PBeamSmearing *smear = new PBeamSmearing("beam_smear",
											 "Beam smearing");
    TF1 *mf = new TF1("Uniform", "1", 1.426, 1.635);
    smear->SetReaction(const_cast<char*>("p + d"));
    smear->SetMomentumFunction(mf);
    makeDistributionManager()->Add(smear);

    // Define the output file path
    std::string outputFile = "${PLUTO_OUTPUT}/pd-" + 
							filename + "-" + 
							std::to_string(iter);


    // Convert strings to C-style strings
    char* finalProductsCStr = new char[finalProducts.length() + 1];
    char* outputFileCStr = new char[outputFile.length() + 1];
    std::strcpy(finalProductsCStr, finalProducts.c_str());
    std::strcpy(outputFileCStr, outputFile.c_str());

    // Create and configure the reaction
    PReaction myReaction(1.5305, "p", "d", finalProductsCStr, 
						 outputFileCStr, 1, 0, 0, 0);
    
    myReaction.Print();
    myReaction.Loop(1000000);

    // Clean up dynamically allocated memory
    delete[] finalProductsCStr;
    delete[] outputFileCStr;
}
