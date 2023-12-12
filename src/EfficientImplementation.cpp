#include "EfficientImplementation.h"

#include <fstream>
#include <iostream>
#include <sstream>

DominoLine::DominoLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath) {
    loadDominoes(startingDominoPath, true);
    loadDominoes(dominoCollectionPath, false);
}
void DominoLine::loadDominoes(const std::string& path, bool startingDomino) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        std::size_t delimiterPos = line.find(':');
        if (delimiterPos == std::string::npos) {
            std::cerr << "Invalid format in file: " << line << " (Delimiter position: " << delimiterPos << ")" << std::endl;
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
            remainingDominoes[left].push_back(domino);
            if (left != right) {
                remainingDominoes[right].push_back(domino);
            }
        }
    }
}

bool DominoLine::addDominoToLeftOrRight() {
    if (line.empty()) {
        std::cerr << "Error: Line is empty, no starting domino." << std::endl;
        return false;
    }

    if (remainingDominoes.empty()) {
        std::cout << "No remaining dominoes to add." << std::endl;
        return false;
    }

    std::string leftValue = line.front().left;
    std::string rightValue = line.back().right;

    std::cout << "Looking for a match for Left: " << leftValue << " and Right: " << rightValue << std::endl;  // Debug print

    // Check and add to the left if a matching domino is found
    auto itLeft = remainingDominoes.find(leftValue);
    if (itLeft != remainingDominoes.end() && !itLeft->second.empty()) {
        if (findAndAddMatchingDomino(leftValue, true)) {
            return true;
        }
    }

    // Check and add to the right if a matching domino is found
    auto itRight = remainingDominoes.find(rightValue);
    if (itRight != remainingDominoes.end() && !itRight->second.empty()) {
        if (findAndAddMatchingDomino(rightValue, false)) {
            return true;
        }
    }

    std::cout << "No matching domino found for Left: " << leftValue << " or Right: " << rightValue << std::endl;
    return false;
}


bool DominoLine::findAndAddMatchingDomino(const std::string& value, bool addToLeft) {
    auto it = remainingDominoes.find(value);
    if (it != remainingDominoes.end() && !it->second.empty()) {
        for (auto& domino : it->second) {
            if ((addToLeft && domino.right == value) || (!addToLeft && domino.left == value)) {
                if (addToLeft) {
                    line.push_front(domino);
                }
                else {
                    line.push_back(domino);
                }
                it->second.erase(std::remove(it->second.begin(), it->second.end(), domino), it->second.end());
                if (it->second.empty()) {
                    remainingDominoes.erase(it);
                }
                return true;
            }
        }
    }
    return false;
}

bool DominoLine::isLineComplete() const {
    return remainingDominoes.empty();
}

void DominoLine::displayLine() const {
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
        DominoLine dominoLine(startingDominoPath, dominoCollectionPath);

        bool addedNewDomino;
        do {
            addedNewDomino = dominoLine.addDominoToLeftOrRight();
            if (addedNewDomino) {
                std::cout << "Added a new domino to the line." << std::endl;
            }
        } while (addedNewDomino);

        if (!dominoLine.isLineComplete()) {
            std::cerr << "Unable to complete the domino line. There might be remaining dominoes that don't fit." << std::endl;
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