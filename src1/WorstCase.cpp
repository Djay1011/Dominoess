#include "WorstCase.h"
#include <fstream>
#include <iostream>
#include <sstream>

WorstCaseLine::WorstCaseLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath) {
    loadDominosFromFile(startingDominoPath, true);
    loadDominosFromFile(dominoCollectionPath, false);
}

void WorstCaseLine::loadDominosFromFile(const std::string& path, bool startingDomino) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    
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
            dominoSequence.push_back(domino);
            startingDomino = false;
        }
        else {
            unmatchedDominos[left].push_back(domino);
            if (left != right) { // Avoid duplicating if both sides are equal
                unmatchedDominos[right].push_back(domino);
            }
        }
    }
}


bool WorstCaseLine::addDomino() {
    if (dominoSequence.empty()) {
        std::cerr << "No starting domino." << std::endl;
        return false;
    }

    std::string leftValue = dominoSequence.front().leftSide;
    std::string rightValue = dominoSequence.back().rightSide;

    if (findAndAddMatchingDomino(leftValue, true) || findAndAddMatchingDomino(rightValue, false)) {
        return true;
    }

    return false;
}

bool WorstCaseLine::findAndAddMatchingDomino(const std::string& value, bool addToLeft) {
    auto it = unmatchedDominos.find(value);
    if (it != unmatchedDominos.end()) {
        auto& dominoes = it->second;
        for (auto dominoIt = dominoes.begin(); dominoIt != dominoes.end(); ++dominoIt) {
            if ((addToLeft && dominoIt->rightSide == value) || (!addToLeft && dominoIt->leftSide == value)) {
                if (addToLeft) {
                    dominoSequence.insert(dominoSequence.begin(), *dominoIt);
                }
                else {
                    dominoSequence.push_back(*dominoIt);
                }
                dominoes.erase(dominoIt);
                if (dominoes.empty()) {
                    unmatchedDominos.erase(it);
                }
                return true;
            }
        }
    }
    return false;
}

bool WorstCaseLine::isLineComplete() const {
    for (const auto& pair : unmatchedDominos) {
        if (!pair.second.empty()) {
            return false;
        }
    }
    return true;
}

void WorstCaseLine::displayLine() const {
    for (const auto& domino : dominoSequence) {
        std::cout << domino.leftSide << "-" << domino.rightSide << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <starting_domino_file> <domino_collection_file>" << std::endl;
        return 1;
    }
    try {
        WorstCaseLine dominoLine(argv[1], argv[2]);
        while (!dominoLine.isLineComplete()) {
            if (!dominoLine.addDomino()) {
                std::cerr << "No matching domino found to add to the line." << std::endl;
                break;
            }
        }
        dominoLine.displayLine();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}