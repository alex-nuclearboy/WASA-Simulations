#include "../include/data_writer.h"
#include <iostream>
#include <fstream>

DataWriter::DataWriter(const std::string& file_name)
{
    file = new TFile(file_name.c_str(), "RECREATE");
    if (!file || !file->IsOpen()) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
    }
}

DataWriter::~DataWriter() 
{
    if (file) {
        file->Write();
        file->Close();
        delete file;
    }
}

void DataWriter::writeTree(TGraph* graph) 
{
    if (!file || !file->IsOpen() || !graph) {
        std::cerr << "Error writing results: Invalid file or graph pointer." << std::endl;
        return;
    }
    graph->Write();
}

void DataWriter::writeProtonData(const std::vector<std::pair<double, double>>& data, const std::string& file_name) {
    std::ofstream out_file(file_name.c_str());
    if (!out_file.is_open()) {
        std::cerr << "Failed to open text file for writing: " << file_name << std::endl;
        return;
    }

    for (const auto& pair : data) {
        out_file << pair.first << "\t" << pair.second << "\n";
    }

    out_file.close();
}

