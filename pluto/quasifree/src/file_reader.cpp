#include "file_reader.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>

FileReader::FileReader(const std::string& file_path) : file_path(file_path) {}

TGraph* FileReader::read_data() 
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
        if (!(iss >> m >> p)) { break; } // error

        momentum.push_back(m);
        probability.push_back(p);
    }

    file.close();
    return new TGraph(momentum.size(), &momentum[0], &probability[0]);
}
