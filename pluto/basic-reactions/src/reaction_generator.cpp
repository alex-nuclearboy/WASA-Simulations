/**
 * File:         reaction_generator.cpp
 * Author:       Aleksander Khreptak <aleksander.khreptak@alumni.uj.edu.pl>
 * Created:      25 Jan 2024
 * Last updated: 02 Feb 2024
 * 
 * Description:
 * This file implements the ReactionGenerator class, which is designed to simulate nuclear
 * reactions within the context of the WASA-at-COSY experiment. It utilizes the PLUTO
 * framework for simulating the proton-deuteron fusion reactions. 
 * The implementation ensures compatibility with older C++ standards, 
 * specifically targeting environments like Ubuntu 12, thus adhering to 
 * constraints on using modern C++ features.
*/

#include "reaction_generator.h"
#include <iostream>
#include <sstream>
#include <string>
#include <PReaction.h>
#include <PUtils.h>
#include <time.h>

const double ReactionGenerator::p_beam_lower = 1.426;
const double ReactionGenerator::p_beam_upper = 1.635;

ReactionGenerator::ReactionGenerator() : smear(NULL), momentum_function(NULL), 
    angular_function(NULL) 
{
    // Beam Smearing Setup
    smear = new PBeamSmearing(const_cast<char*>("beam_smear"),
                              const_cast<char*>("Beam smearing"));
    momentum_function = new TF1(const_cast<char*>("Uniform"), 
                                const_cast<char*>("1"), 
                                p_beam_lower, p_beam_upper);
    angular_function = new TF1(const_cast<char*>("Gaussian"),
                               const_cast<char*>("gaus"),
                               -TMath::Pi(), TMath::Pi());
    angular_function->SetParameters(1.0, 0.0, 0.1);
    smear->SetReaction(const_cast<char*>("p + d"));
    smear->SetMomentumFunction(momentum_function);
    smear->SetAngularSmearing(angular_function);
    makeDistributionManager()->Add(smear);
}

ReactionGenerator::~ReactionGenerator() 
{
    delete smear;
    delete momentum_function;
    delete angular_function;
}

void ReactionGenerator::simulate(const std::string& final_products,
                                 const std::string& file_name, int iter)
{
    unsigned int seed = static_cast<unsigned int>(time(NULL));
    PUtils::SetSeed(seed);
    
    // Define the output file path
    std::ostringstream oss;
    oss << "${PLUTO_OUTPUT}/pd-" << file_name << "-" << iter;
    std::string output_file = oss.str();
    
    // Reaction Setup
    std::ostringstream par;
    par << "_P1 = " << p_beam_lower;
    std::string param = par.str();
    PReaction my_reaction(const_cast<char*>(param.c_str()),
                          const_cast<char*>("p"), const_cast<char*>("d"),
                          const_cast<char*>(final_products.c_str()),
                          const_cast<char*>(output_file.c_str()),
                          1, 0, 0, 0);
    try {
        my_reaction.Print();
        my_reaction.Loop(1000000);
    } catch (const std::exception& e) {
        std::cerr << "Error during simulation: " << e.what() << std::endl;
    }
}
