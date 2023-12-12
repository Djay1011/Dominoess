#pragma once
#ifndef VARIANTSOLU_H
#define VARIANTSOLU_H

#include <list>
#include <string>
#include <utility>
#include <algorithm>
#include <iterator>

class VariantSolu
{
public:
    VariantSolu(const std::string& startingDominoPath, const std::string& dominoCollectionPath);
    void displayLine() const;

private:
    void loadDominoes(const std::string& path, bool isStartingDominoFile);
    void buildLine();

    std::list<std::pair<std::string, std::string>> dominoData; // List A
    std::list<std::pair<std::string, int>> positionList;       // List P
    std::string startingBlueSymbol;
};

#endif // VARIANTSOLU_H

