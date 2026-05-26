#include "URLExtractor.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>

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

void extractURLs(const std::vector<std::string>& lines, const std::set<std::string>& tlds, std::set<std::string>& urls) {
    for (const std::string& line : lines) {
        std::istringstream ss(line);
        std::string token;
        while (ss >> token) {
            while (!token.empty() && (
                token.front() == '(' ||
                token.front() == '[' ||
                token.front() == '*' ||
                token.front() == '"' ||
                token.front() == '\'' ||
                token.front() == '<' ||
                token.front() == '>' ||
                token.front() == '`' ||
                token.front() == '{'
            )) token.erase(token.begin());
            while (!token.empty() && (
                token.back() == ')' ||
                token.back() == ']' ||
                token.back() == ',' ||
                token.back() == '.' ||
                token.back() == '"' ||
                token.back() == '\'' ||
                token.back() == '>' ||
                token.back() == '`' ||
                token.back() == '}' ||
                token.back() == ';' ||
                token.back() == ':' ||
                token.back() == '!'
            )) token.pop_back();
            std::cout << "TOKEN: [" << token << "]" << std::endl;
            if (isURL(token, tlds)) urls.insert(token);
        }
    }
}

bool isURL(const std::string& token, const std::set<std::string>& tlds) {
    std::string t = token;
    if (t.find("https://") == 0) t = t.substr(8);
    else if (t.find("http://") == 0) t = t.substr(7);
    size_t slash = t.find('/');
    if (slash != std::string::npos) t = t.substr(0, slash);
    size_t dotPos = t.rfind('.');
    if (dotPos == std::string::npos) return false;
    if (dotPos == t.size() - 1) return false;
    std::string tld = t.substr(dotPos + 1);
    std::transform(tld.begin(), tld.end(), tld.begin(), ::tolower);
    return tlds.count(tld) > 0;
}

void writeURLOutput(const std::set<std::string>& urls, const std::string& outputFile) {
    std::ofstream out(outputFile);
    for (const std::string& url : urls) out << url << "\n";
}