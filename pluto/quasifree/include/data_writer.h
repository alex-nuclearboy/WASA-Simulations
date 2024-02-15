#ifndef DATAWRITER_H
#define DATAWRITER_H

#include <string>
#include "TFile.h"
#include "TGraph.h"

class DataWriter {
public:
    DataWriter(const std::string& file_name);
    ~DataWriter();
    void writeTree(TGraph* graph);
    void writeProtonData(const std::vector<std::pair<double, double>>& data, const std::string& file_name);

private:
    //std::string file_name;
    TFile* file;
};

#endif // DATAWRITER_H
