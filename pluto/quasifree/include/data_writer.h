/**
 * @file data_writer.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Declaration of the DataWriter class for efficient data handling 
 *        and storage in files.
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

#ifndef DATA_WRITER_H
#define DATA_WRITER_H

#include <string>
#include <vector>
#include "TFile.h"
#include "TGraph.h"
#include "TTree.h"

/**
 * @class DataWriter
 * @brief Manages the output of simulation data to files.
 *
 * Provides functionality for writing different types of simulation data to files. 
 * It supports writing ROOT-based TTree structures and plain text files.
 */
class DataWriter {
public:
    DataWriter();   ///< Default constructor.
    ~DataWriter();  ///< Destructor.

    /**
     * Constructs the file path for the PLUTO simulation output file.
     *
     * @param model_name Name of the model used in the simulation.
     * @param iteration Iteration number of the simulation run.
     * @return String representing the absolute file path for the PLUTO output file.
     */
    static std::string getPlutoFilePath(
        const std::string& model_name, Int_t iteration);

    /**
     * Constructs the path for the file containing calculated simulation values.
     *
     * @param model_name Name of the model used in the simulation.
     * @param iteration Iteration number of the simulation run.
     * @return String representing the absolute file path for the calculated data file.
     */
    static std::string getDataFilePath(
        const std::string& model_name, Int_t iteration);

    /**
     * Constructs the file path for storing proton momentum and scattering angle data.
     *
     * @param model_name Name of the model used in the simulation.
     * @param iteration Iteration number of the simulation run.
     * @return String representing the absolute file path for the proton data file.
     */
    static std::string getProtonFilePath(
        const std::string& model_name, Int_t iteration);

    /**
     * Writes a TTree structure to a ROOT file.
     *
     * The function creates a ROOT file in "RECREATE" mode, ensuring that any 
     * existing file with the same name is overwritten.
     *
     * @param tree Pointer to the ROOT TTree structure to be written.
     * @param file_name Path to the file where the TTree should be written.
     */
    void writeTreeToFile(TTree* tree, const std::string& file_name);

    /**
     * Writes proton momentum and scattering angle data to a plain text file.
     *
     * This function exports a vector of pairs, where each pair represents effective
     * proton momentum and scattering angle, to a specified text file. 
     * The data is written in a tab-separated format, with each pair on a new line.
     *
     * @param data Vector of pairs, where the first element represents the proton 
     *             momentum and the second element represents the scattering angle.
     * @param file_name Path to the text file to which the data should be written.
     */
    void writeProtonData(
        const std::vector<std::pair<Double_t, Double_t> >& data, 
        const std::string& file_name);

private:
    /**
     * Converts a relative file path to an absolute path.
     *
     * @param relative_path Relative file path to be converted.
     * @return String representing the absolute path of the given file.
     */
    static std::string getAbsolutePath(const std::string& relative_path);
};

#endif // DATA_WRITER_H
