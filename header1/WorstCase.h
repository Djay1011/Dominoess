#pragma once

#ifndef WORST_CASE_H
#define WORST_CASE_H

#include <map>
#include <vector>
#include <string>

class WorstCase {
public:
    std::string leftSide;
    std::string rightSide;

    WorstCase(std::string left, std::string right) : leftSide(left)), rightSide(right)) {}
};

class WorstCaseLine {
private:
    std::vector<WorstCase> dominoSequence;
    std::map<std::string, std::vector<WorstCase>> unmatchedDominos;

public:
    WorstCaseLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath);
    bool addDominoToLeftOrRight();
    bool isLineComplete() const;
    void displayLine() const;

private:
    void loadDominosFromFile(const std::string& path, bool startingDomino);
    bool findAndAddMatchingDomino(const std::string& value, bool addToLeft);
};

#endif // WORST_CASE_H