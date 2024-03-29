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
    VariantSolu(const std::string& startingDominoPath, const std::string& dominoCollectionPath, bool buildImmediately = true);
    void displayLine() const;
    void buildDominoLine();
private:
    void loadDominosFromFile(const std::string& path, bool isStartingDominoFile);


    std::list<std::pair<std::string, std::string>> dominoPairs; // List A
    std::list<std::pair<std::string, int>> positionTrackingList;       // List P
    std::string initialBlueSymbol;
};

#endif // VARIANTSOLU_H

