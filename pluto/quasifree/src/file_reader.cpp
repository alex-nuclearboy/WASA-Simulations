/**
 * @file file_reader.cpp
 * @brief Implementation of the FileReader class used to read nucleon momentum 
 *        distribution data from a file and construct a TGraph object.
 *
 * @details
 * Implements FileReader class methods to open a specified file, read nucleon
 * momentum and probability data, and construct a TGraph object representing
 * this data.
 */

#include "file_reader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <map>

FileReader::FileReader(const std::string& file_path) : file_path(file_path) {}

TGraph* FileReader::readData() 
{
    std::ifstream file(this->file_path.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + file_path);
    }

    std::vector<double> momentum;
    std::vector<double> probability;
    std::string line;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double m, p;
        if (!(iss >> m >> p)) { break; } // Handle error or malformed line

        momentum.push_back(m);
        probability.push_back(p);
    }

    file.close();
    return new TGraph(momentum.size(), &momentum[0], &probability[0]);
}

TGraph* FileReader::loadMomentumDistribution(const std::string& model_name) 
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

    FileReader file_reader(model_to_file_map[model_name]);

    return file_reader.readData();
}
