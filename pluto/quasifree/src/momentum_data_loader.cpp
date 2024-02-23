/**
 * @file momentum_data_loader.cpp
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Implementation of MomentumDataLoader class methods.
 *
 * This class encapsulates the detailed procedures for accessing, reading, and 
 * processing momentum and probability data from external files, translating 
 * it into TGraph objects for use in nuclear physics simulations. It includes 
 * methods tailored for specific nuclear configurations and models.
 *
 * Key functionalities include:
 * - Opening and validating data files based on path and model specifications.
 * - Parsing file contents to extract momentum and probability pairs.
 * - Dynamically constructing TGraph objects representing nucleon momentum 
 *   distributions for various potentials and configurations.
 * - Supporting custom scenarios, including nucleon momentum distributions 
 *   within a deuteron for Paris and CD-Bonn potentials, and proton and N*(1535) 
 *   resonance distributions within ^3He for defined binding energies.
 *
 * @version 2.0
 * @date 2024-02-23
 * 
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 */

#include "momentum_data_loader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <map>
#include "TROOT.h"
#include "Rtypes.h"
#include "TGraph.h"

// Constructor: Initialises the data loader with a specified file path.
MomentumDataLoader::MomentumDataLoader(const std::string& file_path) 
    : file_path(file_path) {}

// Reads momentum and probability data from a file, constructing a TGraph.
TGraph* MomentumDataLoader::readData()
{
    std::ifstream file(this->file_path.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + file_path);
    }

    std::vector<Double_t> momentum;
    std::vector<Double_t> probability;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Double_t m, p;
        if (!(iss >> m >> p)) { break; }    // Skips malformed lines

        momentum.push_back(m);
        probability.push_back(p);
    }

    file.close();
    return new TGraph(momentum.size(), &momentum[0], &probability[0]);
}

// Loads the momentum distribution of a nucleon within a deuteron 
// based on the specified potential model.
TGraph* MomentumDataLoader::loadDeuteronNMD(const std::string& model_name) 
{
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
        return NULL;
    }

    MomentumDataLoader file_reader(model_to_file_map[model_name]);
    return file_reader.readData();
}

// Loads the momentum distribution of a proton or N*(1535) resonance 
// for a given energy.
TGraph* MomentumDataLoader::loadHeliumNMD(
    const std::string& particle_type, double energy) 
{
    // Maps particle types and energies to their corresponding file paths.
    std::map<std::string, std::string> particle_file_map;
    particle_file_map["proton"] = 
        "../momentum_distributions/mom_distr_nucleon_3he_converted.txt";

    std::map<double, std::string> energy_to_file_map;
    energy_to_file_map.insert(std::make_pair(0.33, 
        "../momentum_distributions/mom_distr_resonance_3he_e33_converted.txt"));
    energy_to_file_map.insert(std::make_pair(0.53, 
        "../momentum_distributions/mom_distr_resonance_3he_e53_converted.txt"));
    energy_to_file_map.insert(std::make_pair(0.74, 
        "../momentum_distributions/mom_distr_resonance_3he_e74_converted.txt"));

    std::string distribution_file_path;

    if (particle_type == "resonance" 
        && energy_to_file_map.find(energy) != energy_to_file_map.end()) {
        distribution_file_path = energy_to_file_map[energy];
    } else if (particle_file_map.find(particle_type) != particle_file_map.end()) {
        distribution_file_path = particle_file_map[particle_type];
    } else {
        std::cerr << "Error: No file associated with " << particle_type << " ."
                  << "Available options are 'proton' and 'resonance' with "
                  << "specific energies (0.33, 0.53 or 0.74 MeV)." << std::endl;
        return NULL;
    }

    MomentumDataLoader file_reader(distribution_file_path);
    return file_reader.readData();
}
