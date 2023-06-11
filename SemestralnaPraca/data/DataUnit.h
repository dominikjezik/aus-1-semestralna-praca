#pragma once
#include <libds/heap_monitor.h>
#include <iostream>
#include <iomanip>
#include <string>

/**
 * DataUnit reprezentuje zakladnu triedu vsetkych uzemnych jednotiek.
 * Obsahuje vsetky atributy, ktore maju kraje, okresy a obce.
 */
class DataUnit
{
private:
	int sort_number_ = 0;
	std::string code_;
	std::string official_title_;
	std::string medium_title_;
	std::string short_title_;
	std::string note_;
	int vowels_count_ = -1;

protected:
	float population_density_ = 0;

public:
	/**
	 * Predvoleny konstruktor.
	 */
	DataUnit() = default;


	/**
	 * Konstruktor, nastavi nazov uzemnej jednotky podla zadaneho nazvu
	 *
	 * @param official_title Nazov uzemnej jednotky.
	 */
	DataUnit(const std::string& official_title);


	/**
	 * Porovna aktualny objekt typu DataUnit s inym objektom typu DataUnit pomocou atributov.
	 *
	 * @param other Referencia na objekt s ktorym porovnavame tento objekt.
	 * @return boolean podla toho ci sa rovnaju.
	 */
	bool operator==(const DataUnit& other) const;


	/**
	 * Priradenie hodnot z ineho objektu DataUnit do aktualneho pomocou operatora =.
	 *
	 * @param other Referencia na druhy objekt DataUnit, z ktorej budu hodnoty priradene.
	 * @return Referencia na aktualnu instanciu DataUnit.
	 */
	DataUnit& operator=(const DataUnit& other);


	/**
	 * Metoda na zaklade vstupneho pola nastavi vsetky
	 * atributy uzemnej jednotky.
	 */
	void parseData(std::string line[]);


	/**
	 * Metoda vypise riadkovy vypis s informaciami o uzemnej jednotke.
	 */
	void print() const;


	/**
	 * Metoda podrobne vypise vsetky informacie o uzemnej jednotke.
	 */
	virtual void printDetails() const;


	// ----- Getters na vsetky atributy -----
	std::string official_title() const;

	std::string medium_title() const;

	std::string short_title() const;

	std::string code() const;

	std::string note() const;

	int sort_number() const;

	float population_density() const;


	/**
	 * Getter poctu samohlasok. Pri prvom volani vypocita z nazvu pocet
	 * samohlasok a vysledok ulozi do atributu. Pri dalsich volaniach
	 * tohto gettera vrati bez vypoctu hodnotu atributu.
	 *
	 * @return Pocet samohlasok v nazve.
	 */
	int vowels_count();


	/**
	 * Metoda je tu uvedena, len kvoli tomu aby sme mohli
	 * pouzivat dynamic cast na potomkov tejto triedy.
	 */
	virtual void dummy() = 0;

};


/**
 * Trieda AggregatedDataUnit rozsiruje zakladnu triedu DataUnit
 * a prezentuje uzemnu jednotku, ktora poskytuje agregovane
 * udaje. Konkretne eviduje priemernu hustotu obcii,
 * ktore do nej patria.
 */
class AggregatedDataUnit : public DataUnit
{
private:
	int count_of_obce_ = 0;
	float sum_population_density_ = 0;

public:
	/**
	 * Predvoleny konstruktor.
	 */
	AggregatedDataUnit() = default;


	/**
	 * Konstruktor, nastavi nazov uzemnej jednotky podla zadaneho nazvu
	 *
	 * @param official_title Nazov uzemnej jednotky.
	 */
	AggregatedDataUnit(const std::string& official_title);


	/**
	 * Metoda podrobne vypise vsetky informacie o uzemnej jednotke.
	 * Rovnaky vypis ako v predkovy ale navyse prida informaciu
	 * o priemernej hustote obyvatelstva na obec.
	 */
	void printDetails() const override;


	/**
	 * Metoda vypise agregovane informacie, teda 
	 * primernu hustotu obyvatelstva na obec.
	 */
	void printAggregatedData() const;


	/**
	 * Metoda aktualizuje priemernu hustotu obyvatelstva
	 * na obec o zadanu hodnotu, ktora prislucha
	 * dalsej obci.
	 *
	 * @param pd hodnota hustoty obyvatelstva obce
	 */
	void addPopulationDensity(float pd);


	/**
	 * Metoda je tu uvedena, len kvoli tomu aby sme mohli
	 * pouzivat dynamic cast na potomkov a predka tejto triedy.
	 */
	void dummy() override { }

};


/**
 * Trieda Obec dedi z triedy DataUnit a pridava navyse moznost
 * nastavit hustotu obyvatelstva.
 */
class Obec : public DataUnit
{
public:
	/**
	 * Metoda podrobne vypise vsetky informacie o uzemnej jednotke.
	 * Rovnaky vypis ako v predkovy ale navyse prida informaciu
	 * o hustote obyvatelstva v danej obci.
	 */
	void printDetails() const override;


	// ----- Setters -----
	void population_density(float pd);


	/**
	 * Metoda je tu uvedena, len kvoli tomu aby sme mohli
	 * pouzivat dynamic cast na potomkov a predka tejto triedy.
	 */
	void dummy() override { }

};


class Okres : public AggregatedDataUnit
{
	/**
	 * Metoda je tu uvedena, len kvoli tomu aby sme mohli
	 * pouzivat dynamic cast na potomkov a predka tejto triedy.
	 */
	void dummy() override { }
};


class Kraj : public AggregatedDataUnit
{
	/**
	 * Metoda je tu uvedena, len kvoli tomu aby sme mohli
	 * pouzivat dynamic cast na potomkov a predka tejto triedy.
	 */
	void dummy() override { }
};
