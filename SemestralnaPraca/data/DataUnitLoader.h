#pragma once
#include <libds/heap_monitor.h>
#include "CsvLoader.h"
#include "DataUnit.h"
#include "PopulationDensity.h"
#include "libds/amt/explicit_hierarchy.h"
#include "libds/adt/queue.h"

class DataUnitLoader
{
private:
	CsvLoader csvLoader_;
	ds::amt::MultiWayExplicitHierarchy<DataUnit*>* hierarchy_ = nullptr;
	ds::adt::ExplicitQueue<PopulationDensity*> populationDensities;
	std::string rootTitle_;
	std::string krajeFilePath_;
	std::string okresyFilePath_;
	std::string obceFilePath_;
	std::string hustotaFilePath_;

public:
	/**
	 * Konstruktor triedy, ktory ako vstupny parameter prebera ukazovatel na hierarchiu,
	 * do ktorej ma prebehnut nacitanie uzemnych jednotiek Slovenskej Republiky.
	 *
	 * @param hierarchy Ukazovatel na hierarchiu uzemnych jednotiek
	 * @param rootTitle Nazov korena hierarchie
	 * @param krajeFilePath Cesta k csv suboru s datami o krajoch
	 * @param okresyFilePath Cesta k csv suboru s datami o okresoch
	 * @param obceFilePath Cesta k csv suboru s datami o obciach
	 * @param hustotaFilePath Cesta k csv suboru s datami o hustote obyvatelstva jednotlivych obcii
	 */
	DataUnitLoader(ds::amt::MultiWayExplicitHierarchy<DataUnit*>* hierarchy, std::string rootTitle, 
		std::string krajeFilePath, std::string okresyFilePath, std::string obceFilePath, std::string hustotaFilePath);


	/**
	 * Metoda vlozi koren hiearchie do zadanej hierarchie,
	 * s nazvom zadanym v konstruktore.
	 */
	void insertRoot() const;


	/**
	 * Metoda nacita zo zadaneho suboru kraje, namapuje ich do objektov typu Kraj,
	 * pricom kraje uklada do zadanej hierarchie. V kazdom kroku nacitania
	 * spusta zadanu funkciu, v ktorom spristupni nacitany Kraj.
	 *
	 * @param loadCallback Funkcia, ktora sa vykona pre kazdy nacitany Kraj.
	 */
	void loadKraje(std::function<void(Kraj*)> loadCallback) const;


	/**
	 * Metoda nacita zo zadaneho suboru okresy, namapuje ich do objektov typu Okres,
	 * pricom okresy uklada do zadanej hierarchie pre spravny kraj,
	 * pod ktory patri. V kazdom kroku nacitania spusta zadanu
	 * funkciu, v ktorom spristupni nacitany Okres.
	 *
	 * @param loadCallback Funkcia, ktora sa vykona pre kazdy nacitany Okres.
	 */
	void loadOkresy(std::function<void(Okres*)> loadCallback) const;


	/**
	 * Metoda nacita zo zadaneho suboru obce, namapuje ich do objektov typu Obec,
	 * pricom obce uklada do zadanej hierarchie pre spravny okres, pod ktory
	 * patri. V kazdom kroku nacitania spusta zadanu funkciu, v ktorej
	 * spristupni nacitanu Obec. Este pred samotnym nacitanim obcii
	 * spusti metodu na nacitanie hustoty obyvatelstva kazdej obce.
	 * Nasledne pri nacitani obcii, priradi k danej obci aj
	 * informaciu o jej hustote obyvatelstva.
	 *
	 * @param loadCallback Funkcia, ktora sa vykona pre kazdu nacitanu obec.
	 */
	void loadObce(std::function<void(Obec*)> loadCallback);

private:
	/**
	 * Metoda nacita hustoty obyvatelstva zo zadaneho suboru,
	 * namapuje hodnoty so objektov typu PopulationDensity
	 * a objekty vlozi do fontu.
	 */
	void loadPopulationDensities();
};
