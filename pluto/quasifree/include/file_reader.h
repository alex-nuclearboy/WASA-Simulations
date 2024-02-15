#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include "TGraph.h"

class FileReader {
public:
    explicit FileReader(const std::string& file_path);
    TGraph* read_data();

private:
    std::string file_path;
};

#endif // FILEREADER_H
