#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>
#include <VariantSolu.h>  // Make sure the path is correct

struct PerformanceData {
    double loadTime;
    double buildTime;
};

// This function measures the performance of the VariantSolu class
PerformanceData testPerformance(const std::string& dominoPath) {
    // Measure time taken to create VariantSolu object
    auto start = std::chrono::high_resolution_clock::now();
    VariantSolu variantSolu(dominoPath, dominoPath);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> loadTime = end - start;

    // Measure time taken to build the domino line
    start = std::chrono::high_resolution_clock::now();
    variantSolu.buildDominoLine();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> buildTime = end - start;

    return { loadTime.count(), buildTime.count() };
}

int main() {
    std::vector<std::string> testCases = {"C:/Users/n1145251/Downloads/dominoes-test_data/dominoes-test_data/example/example-starting-domino.txt" };
    std::ofstream reportFile("C:/Users/n1145251/Downloads/dominoes-test_data/dominoes-test_data/example/example-input-uncoloured.txt");

    if (!reportFile) {
        std::cerr << "Failed to open performance_report.txt for writing.\n";
        return 1;
    }

    reportFile << "Test Case, Load Time (ms), Build Time (ms)\n";

    for (const auto& testCase : testCases) {
        try {
            PerformanceData performanceData = testPerformance(testCase);
            double loadTime = performanceData.loadTime;
            double buildTime = performanceData.buildTime;
            reportFile << testCase << ", " << loadTime << ", " << buildTime << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Error processing test case " << testCase << ": " << e.what() << '\n';
        }
    }

    reportFile.close();
    return 0;
}