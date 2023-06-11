#pragma once
#include <libds/heap_monitor.h>
#include <string>

class FilterAlgorithm
{
private:
	std::string searchArg;

public:
	/**
	 * Uroven 1: Univerzalny algoritmus podla zadania
	 *
	 * Algoritmus vyfiltruje prvky struktury urcene interatormi begin
	 * a end podla zadaneho predikatu. Pre kazdy prvok, ktory splna
	 * tento predikat spusti funkciu success, kde ako parameter
	 * posle dany prvok.
	 *
	 * @param begin Iterator na prvy prvok.
	 * @param end Iterator na prvy neplatny prvok.
	 * @param predicate Predikat, ktory sa kontroluje pre kazdy prvok.
	 * @param success Funkcia, ktora sa spusti pre kazdy prvok splnajuci predikat.
	 */
	template<typename T, typename Iterator>
	void filterItemsUsing(
		Iterator begin,
		Iterator end,
		std::function<bool(T&)> predicate,
		std::function<void(T&)> success
	);


	/**
	 * Metoda vola univerzalny algoritmus ale v parameri miesto predikatu prebera
	 * index predikatu, podla ktoreho zvoli predikat z dostupnych v tejto triede.
	 * 
	 * @param begin Iterator na prvy prvok.
	 * @param end Iterator na prvy neplatny prvok.
	 * @param index Oznacenie predikatu z dostupnych predikatov.
	 * @param searchPhrase Vyhladavana fraza respektive argument, ktory pouziva vybrany predikat.
	 * @param success Funkcia, ktora sa spusti pre kazdy prvok splnajuci predikat.
	 */
	template<typename T, typename Iterator>
	void filterItemsUsingPredicateIndex(
		Iterator begin,
		Iterator end,
		std::string index,
		std::string searchPhrase,
		std::function<void(T&)> success
	);

	
private:
	// Dostupne predikaty:
	std::function<bool (DataUnit*)> startsWithStr = [&](const DataUnit* dataUnit)
	{
		const size_t max = searchArg.length();

		if (max > dataUnit->official_title().length())
		{
			return false;
		}

		for (size_t i = 0; i < max; ++i)
		{
			if (dataUnit->official_title().at(i) != searchArg.at(i))
			{
				return false;
			}
		}

		return true;
	};

	std::function<bool(DataUnit*)> containsStr = [&](const DataUnit* dataUnit)
	{
		return dataUnit->official_title().find(searchArg) != std::string::npos;
	};

	std::function<bool(DataUnit*)> containsStr2 = [&](const DataUnit* dataUnit)
	{
		size_t phraseCharPointer = 0;

		if (searchArg.empty())
		{
			return true;
		}

		for (size_t i = 0; i < dataUnit->official_title().length(); ++i)
		{
			if (dataUnit->official_title().at(i) == searchArg.at(phraseCharPointer))
			{
				++phraseCharPointer;
			}
			else
			{
				phraseCharPointer = 0;
			}

			if (phraseCharPointer == searchArg.length())
			{
				return true;
			}
		}

		return false;
	};

	std::function<bool(DataUnit*)> hasType = [&](DataUnit* dataUnit)
	{
		if (searchArg == "kraj")
		{
			const Kraj* krajTyped = dynamic_cast<Kraj*>(dataUnit);
			return krajTyped != nullptr;
		}

		if (searchArg == "okres")
		{
			const Okres* okresTyped = dynamic_cast<Okres*>(dataUnit);
			return okresTyped != nullptr;
		}

		if (searchArg == "obec")
		{
			const Obec* obecTyped = dynamic_cast<Obec*>(dataUnit);
			return obecTyped != nullptr;
		}

		return false;
	};

	std::function<bool(DataUnit*)> minimumPopulationDensity = [&](DataUnit* dataUnit)
	{
		try {
			const float requiredMinimumPopulationDensity = std::stof(searchArg);
			return dataUnit->population_density() >= requiredMinimumPopulationDensity;
		}
		catch (std::exception& err)
		{
			return false;
		}
	};

	std::function<bool(DataUnit*)> maximumPopulationDensity = [&](DataUnit* dataUnit)
	{
		try {
			const float requiredMaximumPopulationDensity = std::stof(searchArg);
			return dataUnit->population_density() <= requiredMaximumPopulationDensity;
		}
		catch (std::exception& err)
		{
			return false;
		}
	};

};

template <typename T, typename Iterator>
void FilterAlgorithm::filterItemsUsing(Iterator begin, Iterator end, std::function<bool(T&)> predicate,
	std::function<void(T&)> success)
{
	while (begin != end)
	{
		if (predicate(*begin))
		{
			success(*begin);
		}
		++begin;
	}
}

template <typename T, typename Iterator>
void FilterAlgorithm::filterItemsUsingPredicateIndex(Iterator begin, Iterator end, std::string index,
	std::string searchPhrase, std::function<void(T&)> success)
{
	if (index == "2")
	{
		this->searchArg = searchPhrase;
		this->filterItemsUsing<T>(begin, end, this->startsWithStr, success);
	}
	else if (index == "3")
	{
		this->searchArg = searchPhrase;
		this->filterItemsUsing<T>(begin, end, this->hasType, success);
	}
	else if (index == "4")
	{
		this->searchArg = searchPhrase;
		this->filterItemsUsing<T>(begin, end, this->minimumPopulationDensity, success);
	}
	else if (index == "5")
	{
		this->searchArg = searchPhrase;
		this->filterItemsUsing<T>(begin, end, this->maximumPopulationDensity, success);
	}
	else
	{
		this->searchArg = searchPhrase;
		this->filterItemsUsing<T>(begin, end, this->containsStr, success);
	}
}
