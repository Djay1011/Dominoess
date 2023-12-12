#pragma once

#ifndef WORST_CASE_H
#define WORST_CASE_H

#include <map>
#include <vector>
#include <string>

class WorstCase {
public:
    std::string left;
    std::string right;

    WorstCase(std::string left, std::string right) : left(left), right(right) {}
};

class WorstCaseLine {
private:
    std::vector<WorstCase> line;
    std::map<std::string, std::vector<WorstCase>> dominoMap;

public:
    WorstCaseLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath);
    bool addDominoToLeftOrRight();
    bool isLineComplete() const;
    void displayLine() const;

private:
    void loadDominoes(const std::string& path, bool startingDomino);
    bool findAndAddMatchingDomino(const std::string& value, bool addToLeft);
};

#endif // WORST_CASE_H