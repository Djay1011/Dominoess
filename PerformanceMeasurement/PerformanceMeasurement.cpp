#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include "EfficientImplementation.h"
#include "WorstCase.h"

template <typename DominoClass>
double measurePerformance(const std::string & startingDominoPath, const std::string & dominoCollectionPath) {
    auto start = std::chrono::high_resolution_clock::now();
    DominoClass dominoInstance(startingDominoPath, dominoCollectionPath);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

int main() {
    std::vector<std::string> startingDominoFiles = { "C:/Users/N1145251/Downloads/dominoes-test_data/dominoes-test_data/6K/6K-starting-domino.txt" };
    std::vector<std::string> dominoCollectionFiles = { "C:/Users/N1145251/Downloads/dominoes-test_data/dominoes-test_data/6K/6K-input-uncoloured.txt" };
    std::vector<std::pair<double, double>> results;
    for (size_t i = 0; i < startingDominoFiles.size(); ++i) {
        double timeTakenDominoLine = measurePerformance<DominoLine>(startingDominoFiles[i], dominoCollectionFiles[i]);
        double timeTakenWorstCase = measurePerformance<WorstCase>(startingDominoFiles[i], dominoCollectionFiles[i]);
        results.emplace_back(timeTakenDominoLine, timeTakenWorstCase);
    }

    std::cout << "DataSet\tDominoLine Time(ms)\tWorstCase Time(ms)\n";
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "Data set " << i + 1 << ":\t" << results[i].first << "\t\t" << results[i].second << std::endl;
    }

    return 0;

}
