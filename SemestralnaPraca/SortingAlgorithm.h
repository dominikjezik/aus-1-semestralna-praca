#pragma once
#include "libds/adt/sorts.h"
#include "libds/amt/implicit_sequence.h"
#include <locale>

class SortingAlgorithm
{
private:
	std::locale localeSlovak;

public:
	/**
	 * Predvoleny konstruktor. Inicializujeme tu std::local na slovenske kodovanie Windows-1250.
	 */
	SortingAlgorithm() : localeSlovak("Slovak_Slovakia.1250") {}

	
	/**
	 * Uroven 4: Triediaci algoritmus podla zadania
	 *
	 * Algoritmus utriedi zadanu sekvenciu na zaklade zadaneho komparatora.
	 *
	 * @param is Implicitna sekvencia, ktora sa ma zoradit.
	 * @param compare Komparator, podla ktoreho algorimus sekvenciu zoradi.
	 */
	template<typename T>
	void sortItemsUsing(
		ds::amt::ImplicitSequence<T>& is,
		std::function<bool(const T&, const T&)> compare
	);


	/**
	 * Metoda vola triediaci algoritumus ale v parameri miesto komparatora prebera
	 * index komparatora, podla ktoreho zvoli komparator z dostupnych v tejto triede.
	 */
	template<typename T>
	void sortItemsUsingComparatorIndex(
		ds::amt::ImplicitSequence<T>& is,
		std::string index
	);


private:
	// Dostupne komparatory:
	std::function<bool(DataUnit* const&, DataUnit* const&)> compareAlphabetical = [&](DataUnit* const& d1, DataUnit* const& d2)
	{
		return localeSlovak(d1->official_title(), d2->official_title());
	};

	std::function<bool(DataUnit* const&, DataUnit* const&)> compareVowelsCount = [&](DataUnit* const& d1, DataUnit* const& d2)
	{
		return d1->vowels_count() < d2->vowels_count();
	};
};


template <typename T>
void SortingAlgorithm::sortItemsUsing(ds::amt::ImplicitSequence<T>& is,
	std::function<bool(const T&, const T&)> compare)
{
	ds::adt::QuickSort<T> quickSort;
	quickSort.sort(is, compare);
}


template <typename T>
void SortingAlgorithm::sortItemsUsingComparatorIndex(ds::amt::ImplicitSequence<T>& is, const std::string index)
{
	if (index == "2")
	{
		this->sortItemsUsing<T>(is, compareVowelsCount);
	}
	else
	{
		this->sortItemsUsing<T>(is, compareAlphabetical);
	}
}
