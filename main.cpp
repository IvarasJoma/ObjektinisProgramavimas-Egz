#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include "TextProcessor.h"
#include "URLExtractor.h"
#include <clocale>

int main() {
    std::setlocale(LC_ALL, "");
    std::string inputFile = "input.txt";
    std::vector<std::string> lines = readLines(inputFile);
    std::string tldFile = "tlds-alpha-by-domain.txt";
    std::set<std::string> tlds = loadTLDs(tldFile);
    std::set<std::string> urls;
    extractURLs(lines, tlds, urls);
    writeURLOutput(urls, "urls.txt");
    std::map<std::string, std::vector<int>> wordMap;
    buildWordMap(lines, wordMap, tlds);
    writeFrequencyOutput(wordMap, "daznis.txt");
    writeCrossReference(wordMap, "crossref.txt");
    return 0;
}