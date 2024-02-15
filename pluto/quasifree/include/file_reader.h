#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include "TGraph.h"

class FileReader {
public:
    explicit FileReader(const std::string& file_path);
    TGraph* read_data();

private:
    std::string file_path;
};

#endif // FILE_READER_H
