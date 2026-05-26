#include "urlExtractor.h"
#include <fstream>
#include <algorithm>

std::set<std::string> loadTLDs(const std::string& tldFile) {
    std::set<std::string> tlds;
    std::ifstream file(tldFile);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        tlds.insert(line);
    }
    return tlds;
}