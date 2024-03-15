#include "shader_utils.hpp"

// std
#include <fstream>
#include <sstream>

std::string readFileContents(const std::string& filePath) {
    // open file
    std::ifstream file;
    file.open(filePath);

    // read file contents
    std::stringstream fileStream;
    fileStream << file.rdbuf();

    // close file
    file.close();

    return fileStream.str();
}
