/**
 * @file file_reader.h
 * @brief Declaration of FileReader class for reading nucleon momentum 
 *        distribution data.
 *
 * This class provides functionality to read data from a specified file 
 * containing nucleon momentum and probability values, and to construct 
 * a TGraph object from this data. The expected file format consists of lines 
 * with two numerical values per line, representing momentum and its 
 * corresponding probability.
 */


#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include "TGraph.h"

class FileReader {
public:
    /**
     * Constructs a FileReader object with the path to the data file.
     * @param file_path Path to the file containing momentum distribution data.
     */
    explicit FileReader(const std::string& file_path);
    
    /**
     * @brief Loads the nucleon momentum distribution data for the specified model.
     * 
     * This function maps model names to their respective momentum distribution 
     * files and reads the data from the specified file. It supports two models: 
     * 'paris' and 'cdbonn', each associated with its own momentum distribution file.
     * 
     * @param model_name The name of the model for which the momentum distribution 
     *                   is to be loaded.
     * @return A pointer to a TGraph object containing the loaded momentum 
     *         distribution data, or NULL if an error occurs.
     * 
     * @note Ensure the momentum distribution files are correctly placed in the 
     *       "../momentum_distributions/" directory.
     */
    static TGraph* loadMomentumDistribution(const std::string& model_name);

private:
    std::string file_path;   ///< Path to the data file

    /**
     * Reads momentum distribution data from the file and constructs a TGraph.
     * @return Pointer to a TGraph object containing the data from the file.
     * @throws std::runtime_error if the file cannot be opened.
     */
    TGraph* readData();
};

#endif // FILE_READER_H
