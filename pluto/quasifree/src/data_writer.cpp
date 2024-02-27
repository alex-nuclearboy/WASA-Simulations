/**
 * @file data_writer.cpp
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Implementation of the DataWriter class for efficient data handling and storage in files.
 * 
 * The DataWriter class is specifically designed to facilitate the storage and
 * management of simulation data in ROOT and text formats. This includes handling
 * outputs for PLUTO simulations, storing calculated data, and recording effective
 * proton momentum and scattering angle data.
 * 
 * @version 2.0
 * @date 2024-02-23
 * 
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
*/

#include "data_writer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <cstdlib>
#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <cstring>
#include "TFile.h"
#include "TTree.h"

DataWriter::DataWriter() {}     ///< Default constructor.

DataWriter::~DataWriter() {}    ///< Destructor.

void DataWriter::writeTreeToFile(TTree* tree, const std::string& file_name) 
{
    // Writes a ROOT TTree structure to a file.

    if (!tree) {
        std::cerr << "No tree provided to write." << std::endl;
        return;
    }
    TFile file(file_name.c_str(), "RECREATE");
    if (!file.IsOpen()) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        return;
    }

    tree->Write();
    file.Close();
}

void DataWriter::writeProtonData(
    const std::vector<std::pair<Double_t, Double_t> >& data, 
    const std::string& file_name) 
{
    // Writes pairs of proton momentum and scattering angle to a text file.

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
    const std::string& model_name, Int_t iteration) 
{
    // Returns the file path for storing the PLUTO simulation outputs, 
    // including the model name and iteration.
    std::ostringstream path;
    path << getenv("PLUTO_OUTPUT") << "/pd-ppn_spec-" << model_name << "-" 
         << (iteration + 1) << ".root";
    return getAbsolutePath(path.str());
}

std::string DataWriter::getDataFilePath(
    const std::string& model_name, Int_t iteration)
{
    // Returns the path for the file containing calculated simulation values,
    // formatted with the model name and iteration.
    std::ostringstream path;
    path << "../data/data_ppn_spec-" << model_name << "-" << (iteration + 1) 
         << ".root";
    return getAbsolutePath(path.str());
}

std::string DataWriter::getProtonFilePath(
    const std::string& model_name, Int_t iteration)
{
    // Returns the file path for storing proton data, 
    // formatted with the model name and iteration.
    std::ostringstream path;
    path << "../data/proton_momentum_theta-" << model_name << "-" 
         << (iteration + 1) << ".txt";
    return getAbsolutePath(path.str());
}

std::string DataWriter::getAbsolutePath(const std::string& relative_path) 
{
    // Retrieves the absolute path for a given relative path.
    char temp[PATH_MAX]; ///< Temporary buffer to store the absolute path.
    char* path_copy = strdup(relative_path.c_str());
    char* dir_path = dirname(path_copy);
    char* resolved_path = realpath(dir_path, temp);
    free(path_copy); // Free the duplicated string
    if (resolved_path) {
        std::string abs_path(resolved_path);
        // Extract the file name and append it to the resolved directory path
        std::string file_name = relative_path.substr(relative_path.find_last_of("/\\") + 1);
        abs_path += "/" + file_name; // Use appropriate separator
        return abs_path;
    } else {
        std::cerr << "Error resolving absolute path for: " << dir_path << std::endl;
        return ""; // Or handle the error as appropriate
    }
}
