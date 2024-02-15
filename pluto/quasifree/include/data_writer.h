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
    void writeTree(TGraph* graph);
    void writeProtonData(const std::vector<std::pair<double, double> >& data, const std::string& file_name);
    void writeSimulationData(TTree* tree, const std::string& file_name);

private:
    TFile* file_;
};

#endif // DATA_WRITER_H
