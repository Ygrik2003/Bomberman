#include "fileReader.hpp"

FileReader::FileReader(){
}

FileReader::~FileReader(){
}

std::vector<std::string> FileReader::getLines(std::string filename){
    std::vector<std::string> strings;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return strings;
    }

    std::string line;
    while (std::getline(file, line)) {
        strings.push_back(line);
    }

    file.close();

    return strings;
}

std::string FileReader::readAll(std::string filename){
    std::ifstream fileStream(filename.c_str());
    if (!fileStream.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return "";
    }

    std::string text = "";
    std::string line;
    while(std::getline(fileStream, line)){
        text.append(line + "\n");
    }
    fileStream.close();

    return text;
}
