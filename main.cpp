#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include "textProcessor.h"
#include "urlExtractor.h"

int main() {
    std::string inputFile = "input.txt";
    std::string tldFile = "tlds-alpha-by-domain.txt";
    std::set<std::string> tlds = loadTLDs(tldFile);
    std::vector<std::string> urls;
    extractURLs(inputFile, tlds, urls);
    writeURLOutput(urls, "urls.txt");
    std::map<std::string, std::vector<int>> wordMap;
    buildWordMap(inputFile, wordMap);
    writeFrequencyOutput(wordMap, "daznis.txt");
    writeCrossReference(wordMap, "crossref.txt");
    return 0;
}