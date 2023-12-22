#include <iostream>
#include <fstream>
#include "VariantSolu.h"


VariantSolu::VariantSolu(const std::string& startingDominoPath, const std::string& dominoCollectionPath, bool buildImmediately) {
    loadDominosFromFile(startingDominoPath, true);
    loadDominosFromFile(dominoCollectionPath, false);
    if (buildImmediately) {
        buildDominoLine();
    }
}

// Load dominoes from specified file.
void VariantSolu::loadDominosFromFile(const std::string& path, bool isStartingDominoFile) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);// Throw an error if file fails to open.
    }


    std::string line;
    bool isFirstLine = true;//Process first line only for starting domino file.

    while (std::getline(file, line)) {//Read lines from the file.
        std::size_t delimiterPos = line.find(':');//Find the delimiter in the line.
        if (delimiterPos == std::string::npos) {
            std::cerr << "Warning: Invalid line format in file " << path << ": " << line << std::endl;
            continue; // Skip invalid lines.
        }

        std::string left = line.substr(0, delimiterPos);
        std::string right = line.substr(delimiterPos + 1);

        // Process the starting domino file differently from the collection file.
        if (isStartingDominoFile) {
            initialBlueSymbol = left; // Starting domino's blue symbol
            positionTrackingList.emplace_back(right, 0); // Add to the position tracking list.(List P)
            break;// Only process the first line for the starting domino file.
        }
        else {
            dominoPairs.emplace_back(left, right); // Add domino pair to collection list (List A)
        }
    }
    //Error if necessary data not found in files.
    if (isStartingDominoFile && initialBlueSymbol.empty()) {
        throw std::runtime_error("No starting domino found in file: " + path);
    }
    else if (!isStartingDominoFile && dominoPairs.empty()) {
        throw std::runtime_error("No domino data found in file: " + path);
    }
}

// Method to build the domino line.
void VariantSolu::buildDominoLine() {
    int distance = 1; // Set the distance for domino matching.
    while (distance < static_cast<int>(dominoPairs.size())) {
        //Copy and manipulate domino lists.
        std::list<std::pair<std::string, std::string>> listB = dominoPairs; // Copy of List A
        std::list<std::pair<std::string, std::string>> listC; // List for new matches.

        //Sort the lists by specific matching criteria.
        dominoPairs.sort([](const auto& a, const auto& b) { return a.second < b.second; });
        listB.sort([](const auto& a, const auto& b) { return a.first < b.first; });
        positionTrackingList.sort([](const auto& a, const auto& b) { return a.first < b.first; });

        //Iterators for lists.
        auto itA = dominoPairs.begin();
        auto itB = listB.begin();
        auto itP = positionTrackingList.begin();

        // Process lists to find and match dominoes.
        while (itB != listB.end()) {
            //Conditions to match and move dominos between lists.
            if (itA != dominoPairs.end() && itB->first == itA->second) {
                listC.emplace_back(itA->first, itB->second);
                ++itA;
                ++itB;
            }
            else if (itP != positionTrackingList.end() && itB->first == itP->first) {
                positionTrackingList.emplace_front(itB->second, itP->second + distance);
                ++itB;
                ++itP;
            }
            // Increment iterators based on conditions.
            else if (itP != positionTrackingList.end() && itB->first > itP->first) {
                ++itP;
            }
            else if (itA != dominoPairs.end() && itB->first > itA->second) {
                ++itA;
            }
            else {
                ++itB;
            }
        }
        dominoPairs = listC;// Update domino pairs list.
        distance *= 2;
    }
    //Sort the position list by distance.
    positionTrackingList.sort([](const auto& a, const auto& b) { return a.second < b.second; });
}


void VariantSolu::displayLine() const {
    std::string blue = initialBlueSymbol; //Start with the initial blue symbol.
    for (const auto& pair : positionTrackingList) {
        std::cout << blue << "-" << pair.first << " "; //Print each domino in order.
        blue = pair.first;// Updates the blue symbol for the next domino.
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <starting_domino_file_path> <domino_collection_file_path>" << std::endl;
        return 1; // Error returned for incorrect arguments.
    }

    try {
        VariantSolu VariantSolu(argv[1], argv[2]);
        VariantSolu.displayLine(); // Display the domino line.
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl; // Catch and display runtime errors
        return 1;
    }
    return 0;
}