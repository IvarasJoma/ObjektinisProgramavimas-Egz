#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H
#include <string>
#include <vector>
#include <map>

void buildWordMap(const std::string& inputFile, std::map<std::string, std::vector<int>>& wordMap);
void writeFrequencyOutput(const std::map<std::string, std::vector<int>>& wordMap, const std::string& outputFile);
void writeCrossReference(const std::map<std::string, std::vector<int>>& wordMap, const std::string& outputFile);
std::string stripPunctuation(const std::string& word);

#endif