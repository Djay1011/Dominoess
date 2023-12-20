#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "EfficientImplementation.h"
#include "WorstCase.h"

// Function to measure task performance.
template <typename TaskClass>
double measurePerformance(const std::string& startingDominoPath, const std::string& dominoCollectionPath) {
    auto start = std::chrono::high_resolution_clock::now();

    TaskClass task(startingDominoPath, dominoCollectionPath); // Instantiate the task.
    while (!task.isLineComplete()) { // Keep looping until task is done.
        if (!task.addDomino()) {
            break; // Exit the loop if no more dominoes can be added.
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;// Find the elapsed time in milliseconds.
    return elapsed.count();
}

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments are provided
    if (argc % 2 != 1 || argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <starting_domino_file1> <domino_collection_file1> [<starting_domino_file2> <domino_collection_file2> ...]" << std::endl;
        return 1;
    }

    // Vectors to store file paths and results
    std::vector<std::string> startingDominoFiles;
    std::vector<std::string> dominoCollectionFiles;
    std::vector<double> resultsB1, resultsB2;

    // Parse command line arguments
    for (int i = 1; i < argc; i += 2) {
        startingDominoFiles.push_back(argv[i]);
        dominoCollectionFiles.push_back(argv[i + 1]);
    }

    // Measure performance for each data set
    for (size_t i = 0; i < startingDominoFiles.size(); ++i) {
        double timeB1 = measurePerformance<DominoLine>(startingDominoFiles[i], dominoCollectionFiles[i]);
        double timeB2 = measurePerformance<WorstCaseLine>(startingDominoFiles[i], dominoCollectionFiles[i]);

        resultsB1.push_back(timeB1);
        resultsB2.push_back(timeB2);

        // Print table header
        std::cout << std::left << std::setw(15) << "\nData Set"
            << std::setw(20) << "Task B1 Time (ms)"
            << std::setw(20) << "Task B2 Time (ms)" << std::endl;
        std::cout << std::setfill('-') << std::setw(55) << "-" << std::endl; // Print a line for table separation
        std::cout << std::setfill(' '); // Reset fill character to space

        // Print each row of the table
        std::cout << std::left << std::setw(15) << ("Data set " + std::to_string(i + 1))
            << std::setw(20) << timeB1
            << std::setw(20) << timeB2 << std::endl;
    }

    return 0;
}