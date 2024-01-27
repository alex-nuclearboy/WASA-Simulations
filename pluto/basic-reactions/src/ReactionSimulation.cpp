#include "ReactionSimulation.h"
#include <PReaction.h>
#include <cstring>
#include <iostream>
#include <random>
#include <chrono>

ReactionSimulation::ReactionSimulation() {
    
    // Beam Smearing Setup
    double sigma = (1.635 - 1.426) / 2; // Половина розмаху діапазону імпульсу
    
    smear = new PBeamSmearing(const_cast<char*>("beam_smear"), 
                              const_cast<char*>("Beam smearing"));
    momentumFunc = new TF1(const_cast<char*>("Uniform"),
                           const_cast<char*>("1"), 1.426, 1.635);
    angularFunc = new TF1(const_cast<char*>("Angular"),
                          const_cast<char*>("gaus"), 
                          -TMath::Pi(), TMath::Pi());
    angularFunc->SetParameters(1.0, 0.0, 0.07);
    smear->SetReaction(const_cast<char*>("p + d"));
    smear->SetMomentumFunction(momentumFunc);
    smear->SetAngularSmearing(angularFunc);
    makeDistributionManager()->Add(smear);
}

ReactionSimulation::~ReactionSimulation() {
    delete smear;
    delete momentumFunc;
    delete angularFunc;
}

void ReactionSimulation::simulate(const std::string& finalProducts,
                                  const std::string& filename,
                                  int iter) {
                                    
    unsigned int seed = static_cast<unsigned int>(
        std::chrono::system_clock::now().time_since_epoch().count());
    PUtils::SetSeed(seed);
    
    // Define the output file path
    std::string outputFile = "${PLUTO_OUTPUT}/pd-" + filename + "-" +
                              std::to_string(iter);
                              
    // Reaction Setup
    PReaction myReaction(const_cast<char*>("_P1 = 1.5305"),
                         const_cast<char*>("p"), const_cast<char*>("d"),
                         const_cast<char*>(finalProducts.c_str()),
                         const_cast<char*>(outputFile.c_str()),
                         1, 0, 0, 0);
                         
    try {
        myReaction.Print();
        myReaction.Loop(1000000);
    } catch (const std::exception& e) {
        std::cerr << "Error during simulation: " << e.what()
                  << std::endl;
    }
}
