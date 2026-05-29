#include "URLExtractor.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <format>

static const std::string STRIP_LEADING = "([*\"'<>`{";
static const std::string STRIP_TRAILING = ")],.\"'>`};:!";

static void stripTokenEdges(std::string& token) {
    while (!token.empty() && STRIP_LEADING.find(token.front()) != std::string::npos)
        token.erase(token.begin());
    while (!token.empty() && STRIP_TRAILING.find(token.back()) != std::string::npos)
        token.pop_back();
}

static std::string toLowerStr(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

static bool hasAlnum(const std::string& s) {
    for (char c : s)
        if (std::isalnum((unsigned char)c)) return true;
    return false;
}

std::set<std::string> loadTLDs(const std::string& tldFile) {
    std::set<std::string> tlds;
    std::ifstream file(tldFile);
    if (!file.is_open()) {
        std::cerr << std::format("Error: could not open TLD file {}\n", tldFile);
        return tlds;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        tlds.insert(toLowerStr(line));
    }
    return tlds;
}

bool isURL(const std::string& token, const std::set<std::string>& tlds) {
    std::string t = token;
    if      (t.starts_with("https://")) t = t.substr(8);
    else if (t.starts_with("http://"))  t = t.substr(7);
    size_t slash = t.find('/');
    if (slash != std::string::npos) t = t.substr(0, slash);
    size_t colon = t.find(':');
    if (colon != std::string::npos) t = t.substr(0, colon);
    size_t dotPos = t.rfind('.');
    if (dotPos == std::string::npos) return false;
    if (dotPos == 0)                 return false;  // leading dot
    if (dotPos == t.size() - 1)     return false;  // trailing dot
    std::string domain = t.substr(0, dotPos);
    if (!hasAlnum(domain)) return false;  // rejects "...is" style false positives
    std::string tld = toLowerStr(t.substr(dotPos + 1));
    return tlds.count(tld) > 0;
}

void extractURLs(const std::vector<std::string>& lines, const std::set<std::string>& tlds, std::set<std::string>& urls) {
    for (const std::string& line : lines) {
        std::istringstream ss(line);
        std::string token;
        while (ss >> token) {
            stripTokenEdges(token);
            if (isURL(token, tlds)) urls.insert(token);
        }
    }
}

void writeURLOutput(const std::set<std::string>& urls, const std::string& outputFile) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << std::format("Error: could not open {}\n", outputFile);
        return;
    }
    for (const std::string& url : urls) out << url << "\n";
}