#include <chrono>
#include <iostream>
#include <string>
#include "VariantSolu.h"

double measurePerformanceVariantSolu(const std::string& startingDominoPath, const std::string& dominoCollectionPath) {
    auto start = std::chrono::high_resolution_clock::now();

    try {
        VariantSolu variantSolu(startingDominoPath, dominoCollectionPath);
        variantSolu.displayLine(); // Assuming you want to display the line as well
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return -1.0; // Return a negative value to indicate failure
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <starting_domino_file_path> <domino_collection_file_path>" << std::endl;
        return 1;
    }

    double performanceTime = measurePerformanceVariantSolu(argv[1], argv[2]);
    if (performanceTime >= 0.0) {
        std::cout << "Performance Time: " << performanceTime << " ms" << std::endl;
    }
    else {
        std::cout << "An error occurred during performance measurement." << std::endl;
    }

    return 0;
}
