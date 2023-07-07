#include "utils.hpp"
#include <fstream>
#include <sstream>

std::string readFile(const char *path) {
    std::ifstream file(path);
    if (!file.good())
        return "";

    std::stringstream ss;

    ss << file.rdbuf();

    std::string out = ss.str();

    file.close();

    return out;
}
