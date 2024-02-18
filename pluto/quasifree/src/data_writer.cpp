#include "data_writer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
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
