#ifndef DATA_WRITER_H
#define DATA_WRITER_H

#include <string>
#include <vector>
#include "TFile.h"
#include "TGraph.h"
#include "TTree.h"

class DataWriter {
public:
    DataWriter();
    ~DataWriter();

    /**
     * Constructs the file path for the PLUTO output file.
     * @param model_name Name of the model used in the simulation.
     * @param iteration Iteration number of the simulation run.
     * @return Absolute file path for the PLUTO output file.
     */    
    static std::string getPlutoFilePath(
        const std::string& model_name, int iteration);
    
    /**
     * Constructs the file path for the file with calculated values.
     * @param model_name Name of the model used in the simulation.
     * @param iteration Iteration number of the simulation run.
     * @return Absolute file path for the calculated data file.     
     */
    static std::string getDataFilePath(
        const std::string& model_name, int iteration);
    
    /**
     * Constructs the file path for the proton momentum and scattering angle file.
     * @param model_name Name of the model used in the simulation.
     * @param iteration Iteration number of the simulation run.
     * @return Absolute file path for the proton data file.
     */
    static std::string getProtonFilePath(
        const std::string& model_name, int iteration);

    void writeTreeToFile(TTree* tree, const std::string& file_name);
    void writeProtonData(const std::vector<std::pair<double, double> >& data, const std::string& file_name);
    
private:
    /**
     * Retrieves the absolute path of the given relative path.
     * @param relative_path The relative path to be converted.
     * @return Absolute path if resolution is successful, an empty string otherwise.
     */
    static std::string getAbsolutePath(const std::string& relative_path);

    
};

#endif // DATA_WRITER_H
