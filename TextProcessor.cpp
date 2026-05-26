#include "TextProcessor.h"
#include "URLExtractor.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<std::string> readLines(const std::string& inputFile) {
    std::vector<std::string> lines;
    std::ifstream file(inputFile);
    std::string line;
    while (std::getline(file, line)) if (!line.empty()) lines.emplace_back(line);
    return lines;
}

std::string stripPunctuation(const std::string& word) {
    std::string result;
    int i = 0;
    while (i < (int)word.size()) {
        unsigned char c = word[i];
        
        if (c < 0x80) {
            // ASCII — only add if not punctuation
            if (!std::ispunct(c)) {
                result += c;
            }
            i += 1;
        } else if (c < 0xE0) {
            // 2-byte sequence — keep whole thing
            result += word[i];
            result += word[i+1];
            i += 2;
        } else if (c < 0xF0) {
            // 3-byte sequence — keep whole thing
            result += word[i];
            result += word[i+1];
            result += word[i+2];
            i += 3;
        } else {
            // 4-byte sequence — keep whole thing
            result += word[i];
            result += word[i+1];
            result += word[i+2];
            result += word[i+3];
            i += 4;
        }
    }
    return result;
}

void buildWordMap(const std::vector<std::string>& lines,
                  std::map<std::string, std::vector<int>>& wordMap,
                  const std::set<std::string>& tlds) {
    int lineNum = 0;
    for (const std::string& line : lines) {
        lineNum++;
        std::istringstream ss(line);
        std::string token;
        while (ss >> token) {
            if (isURL(token, tlds)) continue;

            std::string word = stripPunctuation(token);
            std::cout << "WORD: [" << word << "] size=" << word.size() << std::endl;
            word = toLower(word);

            if (word.empty()) continue;
            if (word.size() < 2) continue;

            // skip if contains any digit
            bool hasDigit = false;
            for (char c : word) {
                if (std::isdigit((unsigned char)c)) {
                    hasDigit = true;
                    break;
                }
            }
            if (hasDigit) continue;

            // skip if not all letters (handles –, €, → etc.)
            // skip if not all letters
bool allLetters = true;
int i = 0;
while (i < (int)word.size()) {
    uint32_t cp = getCodepoint(word, i);
    if (!isUnicodeLetter(cp)) {
        allLetters = false;
        break;
    }
}
if (!allLetters) continue;

            wordMap[word].push_back(lineNum);
        }
    }
}

std::string toLower(const std::string& word) {
    std::string result;
    int i = 0;
    while (i < (int)word.size()) {
        unsigned char c = word[i];
        
        if (c < 0x80) {
            // ASCII only — safe to tolower
            result += std::tolower(c);
            i += 1;
        } else if (c < 0xE0) {
            // 2-byte sequence — copy untouched
            result += word[i];
            result += word[i+1];
            i += 2;
        } else if (c < 0xF0) {
            // 3-byte sequence — copy untouched
            result += word[i];
            result += word[i+1];
            result += word[i+2];
            i += 3;
        } else {
            // 4-byte sequence — copy untouched
            result += word[i];
            result += word[i+1];
            result += word[i+2];
            result += word[i+3];
            i += 4;
        }
    }
    return result;
}

void writeFrequencyOutput(const std::map<std::string, std::vector<int>>& wordMap,
                           const std::string& outputFile) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Error: could not open " << outputFile << std::endl;
        return;
    }

    out << std::left << std::setw(30) << "Zodis"
        << std::setw(10) << "Kiekis" << "\n";
    out << std::string(40, '-') << "\n";

    for (const auto& entry : wordMap) {
        if (entry.second.size() > 1) {
            out << std::left << std::setw(30) << entry.first
                << std::setw(10) << entry.second.size() << "\n";
        }
    }
}

void writeCrossReference(const std::map<std::string, std::vector<int>>& wordMap,
                          const std::string& outputFile) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Error: could not open " << outputFile << std::endl;
        return;
    }

    out << std::left << std::setw(30) << "Zodis"
        << "Eilutes" << "\n";
    out << std::string(50, '-') << "\n";

    for (const auto& entry : wordMap) {
        if (entry.second.size() > 1) {
            out << std::left << std::setw(30) << entry.first;
            for (int lineNum : entry.second) {
                out << lineNum << " ";
            }
            out << "\n";
        }
    }
}

uint32_t getCodepoint(const std::string& s, int& i) {
    unsigned char c = s[i];
    uint32_t cp;
    
    if (c < 0x80) {           // 1 byte (ASCII)
        cp = c;
        i += 1;
    } else if (c < 0xE0) {    // 2 bytes
        cp = (c & 0x1F) << 6;
        cp |= (s[i+1] & 0x3F);
        i += 2;
    } else if (c < 0xF0) {    // 3 bytes
        cp = (c & 0x0F) << 12;
        cp |= (s[i+1] & 0x3F) << 6;
        cp |= (s[i+2] & 0x3F);
        i += 3;
    } else {                   // 4 bytes
        cp = (c & 0x07) << 18;
        cp |= (s[i+1] & 0x3F) << 12;
        cp |= (s[i+2] & 0x3F) << 6;
        cp |= (s[i+3] & 0x3F);
        i += 4;
    }
    return cp;
}

bool isUnicodeLetter(uint32_t cp) {
    return
        (cp >= 0x0041 && cp <= 0x005A) ||  // A-Z
        (cp >= 0x0061 && cp <= 0x007A) ||  // a-z
        (cp >= 0x00C0 && cp <= 0x024F) ||  // Latin Extended (Lithuanian, Nordic, etc.)
        (cp >= 0x0400 && cp <= 0x04FF) ||  // Cyrillic
        (cp >= 0x0370 && cp <= 0x03FF) ||  // Greek
        (cp >= 0x0590 && cp <= 0x05FF) ||  // Hebrew
        (cp >= 0x0600 && cp <= 0x06FF) ||  // Arabic
        (cp >= 0x4E00 && cp <= 0x9FFF) ||  // CJK (Chinese/Japanese/Korean)
        (cp >= 0xAC00 && cp <= 0xD7AF);    // Hangul (Korean)
}

