#include "data_writer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include "TFile.h"
#include "TTree.h"

DataWriter::DataWriter() {}

DataWriter::~DataWriter() {}
/*
{
    if (file_) {
        file_->Close();
        delete file_;
    }
} */

void DataWriter::writeTreeToFile(TTree* tree, const std::string& file_name) {
    if (!tree) {
        std::cerr << "No tree provided to write." << std::endl;
        return;
    }
    TFile file(file_name.c_str(), "RECREATE", "", 0);
    if (!file.IsOpen()) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        return;
    }

    std::cout << "Current compression level: " << file.GetCompressionLevel() << std::endl;
    
    tree->Write();
    file.Close();
}

void DataWriter::writeProtonData(const std::vector<std::pair<double, double> >& data, const std::string& file_name) {
    std::ofstream out_file(file_name.c_str()); // Use c_str() to convert std::string to const char*
    if (!out_file.is_open()) {
        std::cerr << "Failed to open text file for writing: " << file_name << std::endl;
        return;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        out_file << data[i].first << "\t" << data[i].second << "\n";
    }

    out_file.close();
}


/*


void DataWriter::setBaseFilename(const std::string& baseFilename) {
    this->baseFilename = baseFilename;
}

void DataWriter::setIterationNumber(int iteration) {
    this->iteration = iteration;
}

std::string DataWriter::constructFilename(const std::string& fileType) {
    std::ostringstream filenameStream;

    filenameStream << baseFilename << "-" << iteration;

    if (fileType == "root") {
        filenameStream << ".root";
    } else {
        filenameStream << ".txt";
    }
    return filenameStream.str();

   // std::string extension = fileType == "root" ? ".root" : ".txt"; // Додаємо умову для визначення розширення файлу
   // return baseFilename + "_" + std::to_string(iteration) + extension;
}

void DataWriter::writeData(const std::string& data, const std::string& fileType) {
    if (fileType == "root") {
        // Логіка для роботи з ROOT файлами
        TFile* file = TFile::Open(constructFilename(fileType).c_str(), "RECREATE");
        if (!file || !file->IsOpen()) {
            std::cerr << "Не вдалося відкрити ROOT файл для запису." << std::endl;
            return;
        }

        // Тут може бути код для створення та заповнення TTree або інших ROOT структур
        // Наприклад:
        // TTree* tree = new TTree("tree", "Дерево даних");
        // tree->Fill();
        // tree->Write();
        
        file->Close();
        delete file;
    } else {
        // Звичайний текстовий запис для інших типів файлів
        std::ofstream file(constructFilename(fileType).c_str(), std::ios::app);
        if (file.is_open()) {
            file << data << std::endl;
            file.close();
        } else {
            std::cerr << "Не вдалося відкрити файл для запису." << std::endl;
        }
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

*/