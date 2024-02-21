#include "data_writer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <cstdlib>
#include <limits.h>
#include <unistd.h>
#include "TFile.h"
#include "TTree.h"

DataWriter::DataWriter() {}

DataWriter::~DataWriter() {}

void DataWriter::writeTreeToFile(TTree* tree, const std::string& file_name) {
    if (!tree) {
        std::cerr << "No tree provided to write." << std::endl;
        return;
    }
    TFile file(file_name.c_str(), "RECREATE", "", 1);
    if (!file.IsOpen()) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        return;
    }
    
    tree->Write();
    file.Close();
}

void DataWriter::writeProtonData(const std::vector<std::pair<double, double> >& data, const std::string& file_name) {
    std::ofstream out_file(file_name.c_str());
    if (!out_file.is_open()) {
        std::cerr << "Failed to open text file for writing: " << file_name << std::endl;
        return;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        out_file << data[i].first << "\t" << data[i].second << "\n";
    }

    out_file.close();
}

std::string DataWriter::getPlutoFilePath(
    const std::string& model_name, int iteration) 
{
    std::ostringstream path;
    path << getenv("PLUTO_OUTPUT") << "/pd-ppn_spec-" << model_name << "-" 
         << (iteration + 1) << ".root";
    return getAbsolutePath(path.str());
}

std::string DataWriter::getDataFilePath(
    const std::string& model_name, int iteration)
{
    std::ostringstream path;
    path << "../data/data_ppn_spec-" << model_name << "-" << (iteration + 1) 
         << ".root";
    return getAbsolutePath(path.str());
}

std::string DataWriter::getProtonFilePath(
    const std::string& model_name, int iteration)
{
    std::ostringstream path;
    path << "../data/proton_momentum_theta-" << model_name << "-" 
         << (iteration + 1) << ".txt";
    return getAbsolutePath(path.str());
}

std::string DataWriter::getAbsolutePath(const std::string& relative_path) {
    char temp[PATH_MAX];
    char* resolved_path = realpath(relative_path.c_str(), temp);
    if (resolved_path) {
        return std::string(resolved_path);
    } else {
        std::cerr << "Error resolving absolute path for: " << relative_path 
                  << std::endl;
        return "";
    }
}
