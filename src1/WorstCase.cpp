#include "WorstCase.h"
#include <fstream>
#include <iostream>
#include <sstream>

WorstCaseLine::WorstCaseLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath) {
    loadDominoes(startingDominoPath, true);
    loadDominoes(dominoCollectionPath, false);
}

void WorstCaseLine::loadDominoes(const std::string& path, bool startingDomino) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    std::cout << "Reading file: " << path << std::endl; // Debug print
    std::string line;
    while (getline(file, line)) {
        std::size_t delimiterPos = line.find(':');
        if (delimiterPos == std::string::npos) {
            std::cerr << "Invalid format in file: " << line << std::endl;
            continue;
        }

        std::string left = line.substr(0, delimiterPos);
        std::string right = line.substr(delimiterPos + 1);

        WorstCase domino(left, right);

        if (startingDomino) {
            this->line.push_back(domino);
            startingDomino = false;
        }
        else {
            dominoMap[left].push_back(domino);
            if (left != right) { // Avoid duplicating if both sides are equal
                dominoMap[right].push_back(domino);
            }
        }
    }
}


bool WorstCaseLine::addDominoToLeftOrRight() {
    if (line.empty()) {
        std::cerr << "No starting domino." << std::endl;
        return false;
    }

    std::string leftValue = line.front().left;
    std::string rightValue = line.back().right;

    if (findAndAddMatchingDomino(leftValue, true) || findAndAddMatchingDomino(rightValue, false)) {
        return true;
    }

    return false;
}

bool WorstCaseLine::findAndAddMatchingDomino(const std::string& value, bool addToLeft) {
    auto it = dominoMap.find(value);
    if (it != dominoMap.end()) {
        auto& dominoes = it->second;
        for (auto dominoIt = dominoes.begin(); dominoIt != dominoes.end(); ++dominoIt) {
            if ((addToLeft && dominoIt->right == value) || (!addToLeft && dominoIt->left == value)) {
                if (addToLeft) {
                    line.insert(line.begin(), *dominoIt);
                }
                else {
                    line.push_back(*dominoIt);
                }
                dominoes.erase(dominoIt);
                if (dominoes.empty()) {
                    dominoMap.erase(it);
                }
                return true;
            }
        }
    }
    return false;
}

bool WorstCaseLine::isLineComplete() const {
    for (const auto& pair : dominoMap) {
        if (!pair.second.empty()) {
            return false;
        }
    }
    return true;
}

void WorstCaseLine::displayLine() const {
    for (const auto& domino : line) {
        std::cout << domino.left << "-" << domino.right << " ";
    }
    std::cout << std::endl;
}

/**int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <starting_domino_file_path> <domino_collection_file_path>" << std::endl;
        return 1;
    }

    std::string startingDominoPath = argv[1];
    std::string dominoCollectionPath = argv[2];

    try {
        WorstCaseLine dominoLine(startingDominoPath, dominoCollectionPath);

        while (!dominoLine.isLineComplete()) {
            if (!dominoLine.addDominoToLeftOrRight()) {
                std::cerr << "No matching domino found to add to the line." << std::endl;
                break;
            }
        }

        std::cout << "Final Domino Line: " << std::endl;
        dominoLine.displayLine();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}**/