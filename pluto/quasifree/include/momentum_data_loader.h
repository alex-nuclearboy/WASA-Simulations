/**
 * @file momentum_data_loader.h
 * @author AK <alex.nuclearboy@gmail.com>
 * @brief Declaration of MomentumDataLoader class for reading nucleon momentum 
 *        distribution data.
 *
 * This class provides functionality to read data from a specified file 
 * containing nucleon momentum and probability values, and to construct 
 * a TGraph object from this data. The expected file format consists of lines 
 * with two numerical values per line, representing momentum and its 
 * corresponding probability.
 * 
 * @version 2.0
 * @date 2024-02-21
 *  
 * @note Distributed under the GNU General Public License version 3.0 (GPLv3).
 */

#ifndef MOMENTUM_DATA_LOADER_H
#define MOMENTUM_DATA_LOADER_H

#include <string>
#include "TGraph.h"

/**
 * @class MomentumDataLoader
 * @brief Manages reading of nucleon momentum distribution data from files.
 *
 * This class encapsulates the process of opening a specified file, reading nucleon
 * momentum and probability values, and creating a TGraph object that represents
 * this data. It is designed to work with specific model data files, namely 'paris'
 * and 'cdbonn'.
 */
class MomentumDataLoader {
public:
    /**
     * Constructor that initializes a MomentumDataLoader object with a file path.
     * @param file_path Path to the file containing momentum distribution data.
     */
    explicit MomentumDataLoader(const std::string& file_path);

    /**
     * Loads the nucleon momentum distribution data for a specified model.
     * 
     * Maps model names to corresponding files and reads data to construct
     * a TGraph object. Currently supports 'paris' and 'cdbonn' models.
     * 
     * @param model_name Name of the model whose momentum distribution is loaded.
     * @return Pointer to a TGraph object with momentum distribution data, 
     *         or NULL on error.
     * 
     * @note Momentum distribution files should be in the 
     *       "../momentum_distributions/" directory.
     */
    static TGraph* loadMomentumDistribution(const std::string& model_name);

private:
    std::string file_path;  ///< Path to the data file.

    /**
     * Reads data from the file and constructs a TGraph object.
     * @return Pointer to a TGraph object with file data.
     * @throws std::runtime_error if the file cannot be opened.
     */
    TGraph* readData();
};

#endif // MOMENTUM_DATA_LOADER_H
