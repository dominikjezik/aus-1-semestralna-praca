#pragma once
#include <libds/heap_monitor.h>
#include "libds/adt/list.h"
#include "libds/adt/table.h"

#include "Console.h"
#include "FilterAlgorithm.h"
#include "SortingAlgorithm.h"
#include "data/DataUnit.h"
#include "data/DataUnitLoader.h"

/**
 * Hlavna trieda, ktora riadi priebeh programu.
 */
class App
{
private:
    // Nacitane uzemne jednotky SP-Uroven 1
    ds::adt::ImplicitList<Kraj*> kraje;
    ds::adt::ImplicitList<Okres*> okresy;
    ds::adt::ImplicitList<Obec*> obce;

    // Vyfiltrovane uzemne jednotky SP-Uroven 1
    ds::adt::ImplicitList<Kraj*> spracovaneKraje;
    ds::adt::ImplicitList<Okres*> spracovaneOkresy;
    ds::adt::ImplicitList<Obec*> spracovaneObce;

    // Univerzalny algoritmus SP-Uroven 1
    FilterAlgorithm filterAlgorithm;

    // Vytvorenie hierarchie SP-Uroven 2
    ds::amt::MultiWayExplicitHierarchy<DataUnit*> hierarchy;

    // Spracovane uzemne jednotky z SP-Uroven 2 pouzivane v SP-Uroven 4
    ds::amt::ImplicitSequence<DataUnit*> matchingDataUnits;

    // Tabulky s rozptylenymi zaznamami SP-Uroven 3
    ds::adt::HashTable<std::string, ds::adt::ImplicitList<Kraj*>*>* krajeTable;
    ds::adt::HashTable<std::string, ds::adt::ImplicitList<Okres*>*>* okresyTable;
    ds::adt::HashTable<std::string, ds::adt::ImplicitList<Obec*>*>* obceTable;

    // Triediaci algoritmus SP-Uroven 4
    SortingAlgorithm sortingAlgorithm;

    Console console;
    std::string userInput;
    

public:
    /**
     * Metoda spusti program. Nacita data, inicializuje struktury a zobrazi menu pouzivatelovi.
     */
	void start();


    /**
     * Destruktor. Uvolni dynamicky alokovane objekty.
     */
    ~App();

private:
    /**
     * Metoda inicializuje udajove struktury. Nacita uzemne jednotky do sekcencii, tabuliek a hierarchie.
     * Inicializuje hashovacenie tabulky s danymi hashovacimi funkciami.
     */
    void setup();


    /**
     * Vlozi uzemnu jednotku do danej tabulky, pricom berie do uvahy duplicity. Ak v tabulke este nie je
     * ziadna uzemna jednotka s danym nazvom, vytvori v tabulke na danom kluci implicitny list a donho
     * vlozi uzemnu jednotku. Ak v tabulke uz je uzemna jednotka s danym klucom, ziska implicitny list
     * a vlozi donho uzemnu jednotku.
     *
     * @param dataUnit Uzemna jednotka na vlozenie do tabulky.
     * @param table Tabulka, do ktorej chceme vlozit uzemnu jednotku.
     */
    template <typename T>
    void insertItemIntoTable(T* dataUnit, ds::adt::HashTable<std::string, ds::adt::ImplicitList<T*>*>* table);


    /**
     * Metoda spusti hlavny cyklus, kde zobrazuje pouzivatelovi menu, umoznuje mu
     * navigaciu v programe, zadavat prikazy a zobrazovat udaje.
     */
    void appLoop();


    /**
     * SP-Uroven 1
     *
     * Metoda pouzivatelovi zobrazi menu na vyber predikatu pre filtrovanie uzemnych jednotiek podla
     * zvoleneho predikatu a umozni mu zadat vstupny argument do predikatu. Filtrovanie spusti
     * nad vsetkymi troma typmi uzemnych jednotiek a vysledky ulozi do troch implicitnych
     * listov podla typu uzemnej jednotku. Vysledky vypise pouzivatelovi.
     */
    void sequenceFilterLoop();


    /**
     * SP-Uroven 2 + 4
     *
     * Metoda pouzivatelovi umozni pohyb iteratorom hierachie uzemnych jednotiek, pricom mu vzdy vypise
     * aktualne informacie o jednotke, na ktoru iterator ukazuje a vsetkych synov tejto jednotky.
     * Uzivatel ma moznost sa cislom navigovat na prislusneho syna alebo ist o urovn vyssie
     * cize na svojho otca. Uzivatel ma tiez moznost spustit algoritmus z prvej urovne
     * nad aktuanou podhierarchiou kde ukazuje iterator.
     */
    void hierarchyLoop();


    /**
     * Metodu vola 2 uroven a umoznuje spustit univerzalny algoritmus nad podhierariou, na ktorej
     * koren ukazuje iterator uzemnych jednotiek. Vyzve pouzivatela na zvolenie predikatu a
     * spusti filtrovanie nad touto podhierarchiou. Vypise vysledky a umozni pouzivatelovi
     * zvolit utriedenie vysledkov.
     *
     * @param hierarchy Hierachia uzemnych jednotiek.
     * @param iterator Iteraotr uzemych jednotiek ukazujuci na vrchol korena podhierarchie.
     */
    void runAlgorithm(ds::amt::MultiWayExplicitHierarchy<DataUnit*>& hierarchy, 
        const ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<DataUnit*>>::LevelAccessHierarchyIterator& iterator);


    /**
     * SP-Uroven 3
     *
     * Metoda pouzivatelovi zobrazi menu na vyber tabulky, z ktorej chce vyhladavat
     * informacie o uzemnej jednotke. Nasledne ho vyzve na zadanie nazvu
     * jednotky. Uzemnu jednotku sa pokusy najst v tabulke a pouzivatelovi
     * zobrazi vysledky.
     */
    void tableLoop();


    /**
     * SP-Uroven 4
     *
     * Metoda sa spusti po tom ako pouzivatel potvrdil, ze chce vyfiltrovane uzemne jednotky
     * u z druhej urovne zoradit. Zobrazi mu menu kde si moze vybrat komparator podla,
     * ktoreho chce uzemne jednotky triedit. Spusti triediaci algoritmus so zadanym
     * komparatorm a vypise vysledky na konzolu.
     */
    void sortingLoop();

};


template <typename T>
void App::insertItemIntoTable(T* dataUnit, ds::adt::HashTable<std::string, ds::adt::ImplicitList<T*>*>* table)
{
    ds::adt::ImplicitList<T*>** dataUnitsTableItem = nullptr;
    const auto dataUnitsTableItemExists = table->tryFind(dataUnit->official_title(), dataUnitsTableItem);

    if (!dataUnitsTableItemExists)
    {
        auto list = new ds::adt::ImplicitList<T*>;
        list->insertLast(dataUnit);
        table->insert(dataUnit->official_title(), list);
    }
    else
    {
        (*dataUnitsTableItem)->insertLast(dataUnit);
    }
}
