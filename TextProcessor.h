#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdint>    // ← needed for uint32_t

std::vector<std::string> readLines(const std::string& inputFile);

std::string stripPunctuation(const std::string& word);
std::string toLower(const std::string& word);

uint32_t getCodepoint(const std::string& s, int& i);
bool isUnicodeLetter(uint32_t cp);

void buildWordMap(const std::vector<std::string>& lines,
                  std::map<std::string, std::vector<int>>& wordMap,
                  const std::set<std::string>& tlds);

void writeFrequencyOutput(const std::map<std::string, std::vector<int>>& wordMap,
                          const std::string& outputFile);

void writeCrossReference(const std::map<std::string, std::vector<int>>& wordMap,
                         const std::string& outputFile);

#endif