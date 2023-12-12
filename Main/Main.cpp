#include <iostream>
#include <string>
#include "WorstCase.h"
#include "EfficientImplementation.h"

int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <implementation_type> <starting_domino_file_path> <domino_collection_file_path>" << std::endl;
        return 1;
    }

    std::string implementationType = argv[1];
    std::string startingDominoPath = argv[2];
    std::string dominoCollectionPath = argv[3];

    try {
        if (implementationType == "WorstCase") {
            WorstCaseLine dominoLine(startingDominoPath, dominoCollectionPath);
            while (!dominoLine.isLineComplete()) {
                if (!dominoLine.addDominoToLeftOrRight()) {
                    std::cerr << "No matching domino found to add to the line in WorstCase implementation." << std::endl;
                    break;
                }
            }
            std::cout << "Final Domino Line (WorstCase): " << std::endl;
            dominoLine.displayLine();
        }

        else if (implementationType == "Efficient") {
            DominoLine dominoLine(startingDominoPath, dominoCollectionPath);
            while (!dominoLine.isLineComplete()) {
                if (!dominoLine.addDominoToLeftOrRight()) {
                    std::cerr << "No matching domino found to add to the line in Efficient implementation." << std::endl;
                    break;
                }
            }

            std::cout << "Final Domino Line (Efficient): " << std::endl;
            dominoLine.displayLine();
        }

        else {
            std::cerr << "Invalid implementation type specified. Use 'WorstCase' or 'Efficient'." << std::endl;
            return 1;
        }
    }

    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
