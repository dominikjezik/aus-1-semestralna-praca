#pragma once
#include <string>

class PopulationDensity
{
private:
	std::string code_;
	float value_ = 0;

public:
	/**
	 * Konstruktor triedy, ktory ako vstupny parameter prebera kod obce,
	 * ku ktorej hustota pristulucha a hodnotu.
	 *
	 * @param code Kod prisluchajucej obce.
	 * @param value Hustota obyvatelstva.
	 */
	PopulationDensity(const std::string& code, const float value);


	/**
	 * Porovna aktualny objekt typu PopulationDensity s inym objektom typu PopulationDensity pomocou atributov.
	 *
	 * @param other Referencia na objekt s ktorym porovnavame tento objekt.
	 * @return boolean podla toho ci sa rovnaju.
	 */
	bool operator==(const PopulationDensity& other) const;


	/**
	 * Getter na atribut code.
	 *
	 * @return code Atribut kod obce.
	 */
	std::string code() const;


	/**
	 * Getter na atribut value.
	 *
	 * @return value
	 */
	float value() const;

};
