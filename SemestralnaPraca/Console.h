#pragma once
#include <libds/heap_monitor.h>
#include "libds/amt/explicit_hierarchy.h"
#include <iostream>
#include <string>
#include "data/DataUnit.h"

/**
 * Trieda Console sluzi iba na vypis informacii na konzolu
 * a na nacitanie vstupu od pouzivatela.
 */
class Console
{
    using Iterator =  ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<DataUnit*>>::LevelAccessHierarchyIterator;

public:
    void clear();

    void input(std::string& userInput);


    void mainMenu();

    void hierarchyMenu(Iterator& iterator);

    void filterPredicatesMenu();

    void filterAskForSearchArgument(std::string predicateNumber);

    void printSearchResultsKrajeTitle();

    void printSearchResultsOkresyTitle();

    void printSearchResultsObceTitle();

    void printSearchResultsGeneralTitle();

    template<typename T>
    void printSearchResults(T units);

    template<typename T>
    void printSearchResultsDetails(T units);

    void noResults();

    void waitForUserInput();

    void selectTable();

    void enterKey();

    void noResultKraj();

    void noResultOkres();

    void noResultObec();

    void waitForUserInputAskForSorting();

    void sortingComparatorsMenu();
};


template <typename T>
void Console::printSearchResults(T units)
{
    for (DataUnit* dataUnit : units)
    {
        dataUnit->print();
    }
}


template <typename T>
void Console::printSearchResultsDetails(T units)
{
    for (DataUnit* dataUnit : units)
    {
        std::cout << "----------------------------------------" << std::endl;
        dataUnit->printDetails();
    }
}
