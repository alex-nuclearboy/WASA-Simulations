/**
 * @file momentum_data_loader.cpp
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Implementation of MomentumDataLoader class methods.
 *
 * Defines methods for opening a specified file, reading nucleon momentum and
 * probability data, and constructing a TGraph object to represent this data.
 * 
 * @version 2.0
 * @date 2024-02-21
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
#include "TGraph.h"

MomentumDataLoader::MomentumDataLoader(const std::string& file_path) 
    : file_path(file_path) {}

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
        if (!(iss >> m >> p)) { break; }    // Handle error or malformed line

        momentum.push_back(m);
        probability.push_back(p);
    }

    file.close();
    return new TGraph(momentum.size(), &momentum[0], &probability[0]);
}

TGraph* MomentumDataLoader::loadMomentumDistribution(const std::string& model_name) 
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
