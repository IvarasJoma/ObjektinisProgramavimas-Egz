#include <iostream>
#include <fstream>
#include "textProcessor.h"
#include "urlExtractor.h"

int main() {
    std::string inputFile = "input.txt";
    std::string tldFile = "tlds-alpha-by-domain.txt";
    std::set<std::string> tlds = loadTLDs(tldFile);
    std::map<std::string, std::vector<int>> wordMap;
    buildWordMap(inputFile, wordMap);
    writeFrequencyOutput(wordMap, "daznis.txt");
    writeCrossReference(wordMap, "crossref.txt");
    std::vector<std::string> urls;
    extractURLs(inputFile, urls);
    writeURLOutput(urls, "urls.txt");

    return 0;
}