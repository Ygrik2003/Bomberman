#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

enum FileAccess{
    READ,
    WRITE
};

class FileReader{
    private:
        /* data */
    public:
        FileReader();
        ~FileReader();
        std::vector<std::string> getLines(std::string filename);
        std::string readAll(std::string filename);
};
