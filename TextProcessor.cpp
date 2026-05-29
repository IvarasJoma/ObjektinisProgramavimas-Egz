#include "TextProcessor.h"
#include "URLExtractor.h"
#include <unicode/uchar.h>
#include <unicode/utf8.h>
#include <unicode/unistr.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <format>

bool hasDigits(const std::string& word) {
    for (char c : word)
        if (std::isdigit((unsigned char)c)) return true;
    return false;
}

bool isAllLetters(const std::string& word) {
    int i = 0;
    while (i < (int)word.size()) {
        UChar32 cp;
        U8_NEXT(word.c_str(), i, (int)word.size(), cp);
        if (cp < 0 || !u_isalpha(cp)) return false;
    }
    return true;
}

bool isValidWord(const std::string& word) {
    return word.size() >= 2 && !hasDigits(word) && isAllLetters(word);
}

std::vector<std::string> readLines(const std::string& inputFile) {
    std::vector<std::string> lines;
    std::ifstream file(inputFile);
    std::string line;
    while (std::getline(file, line))
        if (!line.empty()) lines.emplace_back(line);
    return lines;
}

std::string stripPunctuation(const std::string& word) {
    std::string result;
    int i = 0;
    while (i < (int)word.size()) {
        UChar32 cp;
        int prev = i;
        U8_NEXT(word.c_str(), i, (int)word.size(), cp);
        if (cp >= 0x80 || !std::ispunct(static_cast<unsigned char>(cp)))
            result.append(word, prev, i - prev);
    }
    return result;
}

std::string toLower(const std::string& word) {
    icu::UnicodeString us = icu::UnicodeString::fromUTF8(word);
    us.toLower();
    std::string result;
    us.toUTF8String(result);
    return result;
}

void buildWordMap(const std::vector<std::string>& lines, std::map<std::string, std::vector<int>>& wordMap, const std::set<std::string>& tlds) {
    int lineNum = 0;
    for (const std::string& line : lines) {
        lineNum++;
        std::istringstream ss(line);
        std::string token;
        while (ss >> token) {
            if (isURL(token, tlds)) continue;
            std::string word = toLower(stripPunctuation(token));
            if (!isValidWord(word)) continue;
            wordMap[word].push_back(lineNum);
        }
    }
}

void writeFrequencyOutput(const std::map<std::string, std::vector<int>>& wordMap, const std::string& outputFile) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << std::format("Error: could not open {}\n", outputFile);
        return;
    }
    out << std::format("{:<30}{:<10}\n", "Zodis", "Kiekis");
    out << std::string(40, '-') << "\n";
    for (const auto& entry : wordMap) if (entry.second.size() > 1) out << std::format("{:<30}{:<10}\n", entry.first, entry.second.size());
}

void writeCrossReference(const std::map<std::string, std::vector<int>>& wordMap, const std::string& outputFile) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << std::format("Error: could not open {}\n", outputFile);
        return;
    }
    out << std::format("{:<30}{:<20}\n", "Zodis", "Eilutes");
    out << std::string(50, '-') << "\n";
    for (const auto& entry : wordMap) {
        if (entry.second.size() > 1) {
            std::string lineNums;
            for (int lineNum : entry.second)
                lineNums += std::to_string(lineNum) + " ";
            out << std::format("{:<30}{:<20}\n", entry.first, lineNums);
        }
    }
    out.close();
}