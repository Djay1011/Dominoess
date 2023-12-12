#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "VariantSolu.h"

VariantSolu::VariantSolu(const std::string& startingDominoPath, const std::string& dominoCollectionPath) {
    try {
        loadDominoes(startingDominoPath, true);
        loadDominoes(dominoCollectionPath, false);
        buildLine();
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error processing files: ") + e.what());
    }
}

void VariantSolu::loadDominoes(const std::string& path, bool isStartingDominoFile) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }


    std::string line;
    bool isFirstLine = true;

    while (std::getline(file, line)) {
        std::size_t delimiterPos = line.find(':');
        if (delimiterPos == std::string::npos) {
            std::cerr << "Warning: Invalid line format in file " << path << ": " << line << std::endl;
            continue;
        }

        std::string left = line.substr(0, delimiterPos);
        std::string right = line.substr(delimiterPos + 1);

        if (isStartingDominoFile) {
            startingBlueSymbol = left; // Starting domino's blue symbol
            positionList.emplace_back(right, 0); // Add to List P
            break;
        }
        else {
            dominoData.emplace_back(left, right); // Add to List A
        }
    }
    if (isStartingDominoFile && startingBlueSymbol.empty()) {
        throw std::runtime_error("No starting domino found in file: " + path);
    }
    else if (!isStartingDominoFile && dominoData.empty()) {
        throw std::runtime_error("No domino data found in file: " + path);
    }
}


void VariantSolu::buildLine() {
    int distance = 1;
    while (distance < static_cast<int>(dominoData.size())) {
        std::list<std::pair<std::string, std::string>> listB = dominoData; // Copy of List A
        std::list<std::pair<std::string, std::string>> listC;

        dominoData.sort([](const auto& a, const auto& b) { return a.second < b.second; });
        listB.sort([](const auto& a, const auto& b) { return a.first < b.first; });
        positionList.sort([](const auto& a, const auto& b) { return a.first < b.first; });

        auto itA = dominoData.begin();
        auto itB = listB.begin();
        auto itP = positionList.begin();

        while (itB != listB.end()) {
            if (itA != dominoData.end() && itB->first == itA->second) {
                listC.emplace_back(itA->first, itB->second);
                ++itA;
                ++itB;
            }
            else if (itP != positionList.end() && itB->first == itP->first) {
                positionList.emplace_front(itB->second, itP->second + distance);
                ++itB;
                ++itP;
            }
            else if (itP != positionList.end() && itB->first > itP->first) {
                ++itP;
            }
            else if (itA != dominoData.end() && itB->first > itA->second) {
                ++itA;
            }
            else {
                ++itB;
            }
        }
        dominoData = listC;
        distance *= 2;
    }

    positionList.sort([](const auto& a, const auto& b) { return a.second < b.second; });
}


void VariantSolu::displayLine() const {
    std::string blue = startingBlueSymbol;
    for (const auto& pair : positionList) {
        std::cout << blue << "-" << pair.first << " ";
        blue = pair.first;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <starting_domino_file_path> <domino_collection_file_path>" << std::endl;
        return 1;
    }

    try {
        VariantSolu VariantSolu(argv[1], argv[2]);
        VariantSolu.displayLine();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}