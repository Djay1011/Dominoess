#pragma once
#ifndef EFFICIENT_IMPLEMENTATION_H
#define EFFICIENT_IMPLEMENTATION_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <string>


class Domino {
public:
    std::string leftSide;
    std::string rightSide;

    Domino(std::string left, std::string right) : leftSide(std::move(left)), rightSide(std::move(right)) {}

    bool operator==(const Domino& other) const {
        return leftSide == other.leftSide && rightSide == other.rightSide;
    }
};


class DominoLine {
private:
    std::list<Domino> line;
    std::unordered_map<std::string, std::vector<Domino>> availableDominoes;


public:
    DominoLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath);

    bool addDomino();
    bool isLineComplete() const;
    void displayLine() const;

private:
    void loadDominoesFromFile(const std::string& path, bool startingDomino);
    bool findAndAddMatchingDomino(const std::string& matchingSide, bool addToLeft);
};

#endif // EFFICIENT_IMPLEMENTATION_H