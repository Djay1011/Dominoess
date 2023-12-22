#include "EfficientImplementation.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Creates a DominoLine object with specified file paths.
DominoLine::DominoLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath) {
    loadDominoesFromFile(startingDominoPath, true);
    loadDominoesFromFile(dominoCollectionPath, false);
}

// Load dominoes from specified file.
void DominoLine::loadDominoesFromFile(const std::string& path, bool startingDomino) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path); // Throw an error if file fails to open.
    }

    std::string line;
    // Examine each line from the record.
    while (std::getline(file, line)) {
        size_t delimiterPos = line.find(':'); //Find the delimiter position in the line.
        if (delimiterPos == std::string::npos) {
            std::cerr << "Invalid format in file: " << line << std::endl; //Error message for an incorrect format.
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

// Add domino to line
bool DominoLine::addDomino() {
    // Check for empty line and return false if found.
    if (line.empty()) {
        std::cerr << "Error: Line is empty, no starting domino." << std::endl;
        return false;
    }

    // Check for no available dominoes and return false if none found.
    if (availableDominoes.empty()) {
        std::cout << "No more dominoes to add." << std::endl;
        return false;
    }

    // Search for matching dominoes at the ends of the line.
    std::string leftValue = line.front().leftSide;
    std::string rightValue = line.back().rightSide;

    // Debug output with matched values.
    std::cout << "Seeking a suitable pairing for both Left: " << leftValue << " and Right: " << rightValue << std::endl;  // Debug print

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

    // No match found.
    std::cout << "corresponding domino was located for the Left: " << leftValue << " or Right: " << rightValue << std::endl;
    return false;
}

// Find and add a matching domino to the line.
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

// Check if the domino line is complete.
bool DominoLine::isLineComplete() const {
    return availableDominoes.empty(); // Return true if no more dominoes are available.
}

// Display current domino line.
void DominoLine::displayLine() const {
    for (const auto& domino : line) {
        std::cout << domino.leftSide << "-" << domino.rightSide << " "; // Print all dominoes in a line.
    }
    std::cout << std::endl;
}

/*int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <starting_domino_file_path> <domino_collection_file_path>" << std::endl;
        return 1; // Return with error code if incorrect argument.
    }

    std::string startingDominoPath = argv[1];
    std::string dominoCollectionPath = argv[2];

    try {
        // Create a DominoLine object and initiate the line process.
        DominoLine dominoLine(startingDominoPath, dominoCollectionPath);

        bool addedNewDomino;
        // Continues adding dominos until dominoes can't be added anymore.
        do {
            addedNewDomino = dominoLine.addDomino();
            if (addedNewDomino) {
                std::cout << "New domino added to line. \n" << std::endl;
            }
        } while (addedNewDomino);

        // Check if the line is complete and log if it's incomplete.
        if (!dominoLine.isLineComplete()) {
            std::cerr << "Incomplete domino line." << std::endl;
        }

        // Show the final domino line.
        std::cout << "\nFinal Domino Line: " << std::endl;
        dominoLine.displayLine();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl; // Handle exceptions. Display them.
        return 1;
    }

    return 0;
}*/