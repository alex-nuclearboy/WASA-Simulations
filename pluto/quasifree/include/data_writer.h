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

    void writeTreeToFile(TTree* tree, const std::string& file_name);
    void writeProtonData(const std::vector<std::pair<double, double> >& data, const std::string& file_name);


/*


    void writeTree(TGraph* graph);
    void writeProtonData(const std::vector<std::pair<double, double> >& data, const std::string& file_name);
    void writeSimulationData(TTree* tree, const std::string& file_name);

    void setBaseFilename(const std::string& baseFilename);
    void setIterationNumber(int iteration);
    void writeData(const std::string& data, const std::string& fileType);
    
private:
    TFile* file_;

    std::string baseFilename;
    int iteration;
    std::string constructFilename(const std::string& fileType);

    */
};

#endif // DATA_WRITER_H
