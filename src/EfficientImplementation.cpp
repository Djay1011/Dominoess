#include "EfficientImplementation.h"

#include <fstream>
#include <iostream>
#include <sstream>

DominoLine::DominoLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath) {
    loadDominoesFromFile(startingDominoPath, true);
    loadDominoesFromFile(dominoCollectionPath, false);
}
void DominoLine::loadDominoesFromFile(const std::string& path, bool startingDomino) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos == std::string::npos) {
            std::cerr << "Invalid format in file: " << line << std::endl;
            continue;
        }

        std::string left = line.substr(0, delimiterPos);
        std::string right = line.substr(delimiterPos + 1);

        Domino domino(left, right);
        if (startingDomino) {
            this->line.push_back(domino);
            startingDomino = false;
        }
        else {
            availableDominoes[left].push_back(domino);
            if (left != right) {
                availableDominoes[right].push_back(domino);
            }
        }
    }
}

bool DominoLine::addDomino() {
    if (line.empty()) {
        std::cerr << "Error: Line is empty, no starting domino." << std::endl;
        return false;
    }

    if (availableDominoes.empty()) {
        std::cout << "No remaining dominoes to add." << std::endl;
        return false;
    }

    std::string leftValue = line.front().leftSide;
    std::string rightValue = line.back().rightSide;

    std::cout << "Looking for a match for Left: " << leftValue << " and Right: " << rightValue << std::endl;  // Debug print

    // Check and add to the left if a matching domino is found
    auto itLeft = availableDominoes.find(leftValue);
    if (itLeft != availableDominoes.end() && !itLeft->second.empty()) {
        if (findAndAddMatchingDomino(leftValue, true)) {
            return true;
        }
    }

    // Check and add to the right if a matching domino is found
    auto itRight = availableDominoes.find(rightValue);
    if (itRight != availableDominoes.end() && !itRight->second.empty()) {
        if (findAndAddMatchingDomino(rightValue, false)) {
            return true;
        }
    }

    std::cout << "No matching domino found for Left: " << leftValue << " or Right: " << rightValue << std::endl;
    return false;
}


bool DominoLine::findAndAddMatchingDomino(const std::string& value, bool addToLeft) {
    auto it = availableDominoes.find(value);
    if (it != availableDominoes.end() && !it->second.empty()) {
        for (auto& domino : it->second) {
            if ((addToLeft && domino.rightSide == value) || (!addToLeft && domino.leftSide == value)) {
                if (addToLeft) {
                    line.push_front(domino);
                }
                else {
                    line.push_back(domino);
                }
                it->second.erase(std::remove(it->second.begin(), it->second.end(), domino), it->second.end());
                if (it->second.empty()) {
                    availableDominoes.erase(it);
                }
                return true;
            }
        }
    }
    return false;
}

bool DominoLine::isLineComplete() const {
    return availableDominoes.empty();
}

void DominoLine::displayLine() const {
    for (const auto& domino : line) {
        std::cout << domino.leftSide << "-" << domino.rightSide << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <starting_domino_file_path> <domino_collection_file_path>" << std::endl;
        return 1;
    }

    std::string startingDominoPath = argv[1];
    std::string dominoCollectionPath = argv[2];

    try {
        DominoLine dominoLine(startingDominoPath, dominoCollectionPath);

        bool addedNewDomino;
        do {
            addedNewDomino = dominoLine.addDomino();
            if (addedNewDomino) {
                std::cout << "Added a new domino to the line." << std::endl;
            }
        } while (addedNewDomino);

        if (!dominoLine.isLineComplete()) {
            std::cerr << "Incomplete domino line." << std::endl;
        }

        std::cout << "Final Domino Line: " << std::endl;
        dominoLine.displayLine();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}