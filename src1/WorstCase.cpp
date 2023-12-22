#include "WorstCase.h"
#include <fstream>
#include <iostream>
#include <sstream>

WorstCaseLine::WorstCaseLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath) {
    loadDominosFromFile(startingDominoPath, true);
    loadDominosFromFile(dominoCollectionPath, false);
}

// Load dominos from a file.
void WorstCaseLine::loadDominosFromFile(const std::string& path, bool startingDomino) {
    std::ifstream file(path);// Read the file.
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path); // Throws exception when file fails to open.
    }
    
    std::string line;
    while (getline(file, line)) {
        std::size_t delimiterPos = line.find(':');//Find the delimiter in the line.
        if (delimiterPos == std::string::npos) {
            std::cerr << "Invalid format in file: " << line << std::endl; //Error message for an incorrect format.
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

// Add domino to line
bool WorstCaseLine::addDomino() {
    // Check for empty line and return false if found.
    if (dominoSequence.empty()) {
        std::cerr << "No starting domino." << std::endl;
        return false;
    }

    //Find matching values for adding new dominos.
    std::string leftValue = dominoSequence.front().leftSide;
    std::string rightValue = dominoSequence.back().rightSide;

    //Add a domino to either end of the sequence.
    if (findAndAddMatchingDomino(leftValue, true) || findAndAddMatchingDomino(rightValue, false)) {
        return true;
    }

    return false;
}

//Find and add a matching domino to the line.
bool WorstCaseLine::findAndAddMatchingDomino(const std::string& value, bool addToLeft) {
    auto it = unmatchedDominos.find(value);//Find matching dominoes.
    if (it != unmatchedDominos.end()) {
        auto& dominoes = it->second;
        for (auto dominoIt = dominoes.begin(); dominoIt != dominoes.end(); ++dominoIt) {
            //Check domino match and add to sequence.
            if ((addToLeft && dominoIt->rightSide == value) || (!addToLeft && dominoIt->leftSide == value)) {
                if (addToLeft) {
                    dominoSequence.insert(dominoSequence.begin(), *dominoIt); //Add to the beginning if adding to the left.
                }
                else {
                    dominoSequence.push_back(*dominoIt);// Add to the end if adding to the right.
                }
                dominoes.erase(dominoIt); //Remove the extra domino from unmatched dominos.
                if (dominoes.empty()) {
                    unmatchedDominos.erase(it); //Remove empty entries from unmatched dominos.
                }
                return true;
            }
        }
    }
    return false;
}

// Check if the domino line is complete.
bool WorstCaseLine::isLineComplete() const {
    for (const auto& pair : unmatchedDominos) {
        if (!pair.second.empty()) {
            return false;
        }
    }
    return true;
}

// Display current domino line.
void WorstCaseLine::displayLine() const {
    for (const auto& domino : dominoSequence) {
        std::cout << domino.leftSide << "-" << domino.rightSide << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <starting_domino_file> <domino_collection_file>" << std::endl;
        return 1; //Error will be returned for incorrect arguments.
    }
    try {
        // Create a Line instance with file paths.
        WorstCaseLine dominoLine(argv[1], argv[2]);
        while (!dominoLine.isLineComplete()) {// Loop until line is complete.
            if (!dominoLine.addDomino()) {
                std::cerr << "No matching domino found to add to the line." << std::endl;
                break; // Stop the loop if no matching domino can be added.
            }
        }
        dominoLine.displayLine();// Show the last domino line.
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl; // Handle exceptions. Display them.
        return 1;
    }
    return 0;
}