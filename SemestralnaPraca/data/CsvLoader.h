#pragma once
#include <libds/heap_monitor.h>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>

/**
 * Trieda CsvLoader sluzi na nacitanie csv suborov podla zadanej cesty k suboru.
 */
class CsvLoader
{
public:
	/**
	 * Metoda otvori csv subor zo zadanej cesty, kazdy riadok rozdeli podla znaku ';' do
	 * pola segmentov a segmenty spristupni v odovzdanej lambda funkcii ako parameter.
	 *
	 * Casti kodu na nacitanie suboru boli pouzite odtialto https://java2blog.com/read-csv-file-in-cpp/
	 *
	 * @param filePath Cesta k csv suboru.
	 * @param lineAction Funkcia, ktora sa vykona pri kazdom nacitani riadku zo suboru.
	 * @throws Vyhodi vynimku, ak nie je mozne otvorit zadany subor.
	 */
	void load(const std::string& filePath, const std::function<void(std::string[])>& lineAction) const;

private:
	/**
	 * Metoda zoberie vstupny riadok csv suboru a rozdeli ho do dynamicky
	 * alokovaneho pola o velkosti 6 podla znaku ';'.
	 *
	 * @param line Riadok, ktory spracuje do pola.
	 * @return dynamicky alokovane pole segmentov zadaneho riadku
	 */
	std::string* parseLine(const std::string& line) const;
};
