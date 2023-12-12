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
    std::string left;
    std::string right;

    Domino(std::string left, std::string right) : left(left), right(right) {}

    bool operator==(const Domino& other) const {
        return left == other.left && right == other.right;
    }
};


class DominoLine {
private:
    std::list<Domino> line;
    std::unordered_map<std::string, std::vector<Domino>> remainingDominoes;


public:
    DominoLine(const std::string& startingDominoPath, const std::string& dominoCollectionPath);

    bool addDominoToLeftOrRight();
    bool isLineComplete() const;
    void displayLine() const;

private:
    void loadDominoes(const std::string& path, bool startingDomino);
    bool findAndAddMatchingDomino(const std::string& value, bool addToLeft);
};

#endif // EFFICIENT_IMPLEMENTATION_H