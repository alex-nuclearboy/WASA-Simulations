#include "data_writer.h"
#include <iostream>
#include <fstream>
#include <utility>

DataWriter::DataWriter() : file_(NULL) {}

DataWriter::~DataWriter() 
{
    if (file_) {
        file_->Close();
        delete file_;
    }
}

void DataWriter::writeTree(TGraph* graph) 
{
    if (!file_ || !file_->IsOpen() || !graph) {
        std::cerr << "Error writing results: Invalid file or graph pointer." << std::endl;
        return;
    }
    graph->Write();
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

void DataWriter::writeSimulationData(TTree* tree, const std::string& file_name) {
    if (!tree) {
        std::cerr << "No tree provided." << std::endl;
        return;
    }

    TFile* file = new TFile(file_name.c_str(), "RECREATE");
    tree->Write();
    file->Close();
    delete file;
}
