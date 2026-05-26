#ifndef URLEXTRACTOR_H
#define URLEXTRACTOR_H

#include <set>
#include <string>
#include <vector>

std::set<std::string> loadTLDs(const std::string& tldFile);
bool isValidTLD(const std::string& tld, const std::set<std::string>& tlds);
void extractURLs(const std::string& inputFile, const std::set<std::string>& tlds, std::vector<std::string>& urls);
void writeURLOutput(const std::vector<std::string>& urls, const std::string& outputFile);

#endif