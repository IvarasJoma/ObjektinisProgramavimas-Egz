#ifndef URLEXTRACTOR_H
#define URLEXTRACTOR_H

#include <set>
#include <string>
#include <vector>

std::set<std::string> loadTLDs(const std::string& tldFile);
void extractURLs(const std::vector<std::string>& lines, const std::set<std::string>& tlds, std::set<std::string>& urls);
void writeURLOutput(const std::set<std::string>& urls, const std::string& outputFile);
bool isURL(const std::string& token, const std::set<std::string>& tlds);

#endif