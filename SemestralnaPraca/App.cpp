#include "App.h"

void App::start()
{
	this->setup();
	this->appLoop();
}


App::~App()
{
	// Dealokovanie uzemnych jednotiek
	for (size_t i = 0; i < kraje.size(); ++i)
	{
		Kraj* kraj = kraje.access(i);
		delete kraj;
	}

	for (size_t i = 0; i < okresy.size(); ++i)
	{
		Okres* okres = okresy.access(i);
		delete okres;
	}

	for (size_t i = 0; i < obce.size(); ++i)
	{
		Obec* obec = obce.access(i);
		delete obec;
	}

	// Dealokovanie root node hierarchie
	delete hierarchy.accessRoot()->data_;

	// Dealokovanie tabuliek
	for (const auto& obceTableItem : *(this->obceTable))
	{
		delete obceTableItem.data_;
	}

	delete obceTable;

	for (const auto& okresyTableItem : *(this->okresyTable))
	{
		delete okresyTableItem.data_;
	}

	delete okresyTable;

	for (const auto& krajeTableItem : *(this->krajeTable))
	{
		delete krajeTableItem.data_;
	}

	delete krajeTable;
}


void App::setup()
{
	// Hashovacie funkcie
	auto hashFunctionAsciiSum = [](std::string const& key)
	{
		size_t sum = 0;

		for (const char i : key)
		{
			sum += i;
		}

		return sum;
	};

	auto hashFunctionSumOfCharacters = [](std::string const& key) { return key.length(); };

	// Vytvorenie tabuliek
	krajeTable = new ds::adt::HashTable<std::string, ds::adt::ImplicitList<Kraj*>*>(hashFunctionSumOfCharacters, 9);
	okresyTable = new ds::adt::HashTable<std::string, ds::adt::ImplicitList<Okres*>*>(hashFunctionAsciiSum, 50);
	obceTable = new ds::adt::HashTable<std::string,ds::adt::ImplicitList<Obec*>*>(hashFunctionAsciiSum, 2000);

	// Nacitanie dat
	DataUnitLoader loader(&hierarchy, "Slovenská Republika", "input_data/kraje.csv", "input_data/okresy.csv", "input_data/obce.csv", "input_data/hustota-obyvatelstva.csv");
	loader.insertRoot();

	loader.loadKraje([&](Kraj* kraj)
	{
		kraje.insertLast(kraj);
		this->insertItemIntoTable(kraj, krajeTable);
	});

	loader.loadOkresy([&](Okres* okres)
	{
		okresy.insertLast(okres);
		this->insertItemIntoTable(okres, okresyTable);
	});

	loader.loadObce([&](Obec* obec)
	{
		obce.insertLast(obec);
		this->insertItemIntoTable(obec, obceTable);
	});
}


void App::appLoop()
{
	while (true)
	{
		console.mainMenu();
		console.input(userInput);
		console.clear();

		if (userInput == "1")
		{
			this->sequenceFilterLoop();
		}
		else if (userInput == "2")
		{
			this->hierarchyLoop();
		}
		else if (userInput == "3")
		{
			this->tableLoop();
		}
		else if (userInput == "x")
		{
			break;
		}
	}
}


void App::sequenceFilterLoop()
{
	console.filterPredicatesMenu();
	std::string selectedPredicateNumber;
	console.input(selectedPredicateNumber);
	console.clear();

	console.filterAskForSearchArgument(selectedPredicateNumber);
	std::string searchPhrase;
	console.input(searchPhrase);
	console.clear();

	filterAlgorithm.filterItemsUsingPredicateIndex<Kraj*>(
		this->kraje.begin(),
		this->kraje.end(),
		selectedPredicateNumber,
		searchPhrase,
		[&](Kraj* kraj)
		{
			spracovaneKraje.insertLast(kraj);
		});

	filterAlgorithm.filterItemsUsingPredicateIndex<Okres*>(
		this->okresy.begin(),
		this->okresy.end(),
		selectedPredicateNumber,
		searchPhrase,
		[&](Okres* okres)
		{
			spracovaneOkresy.insertLast(okres);
		});

	filterAlgorithm.filterItemsUsingPredicateIndex<Obec*>(
		this->obce.begin(),
		this->obce.end(),
		selectedPredicateNumber,
		searchPhrase,
		[&](Obec* obec)
		{
			spracovaneObce.insertLast(obec);
		});


	if (
		spracovaneObce.size() == 0 &&
		spracovaneOkresy.size() == 0 &&
		spracovaneKraje.size() == 0
	)
	{
		console.noResults();
	}

	if (!spracovaneKraje.isEmpty())
	{
		console.printSearchResultsKrajeTitle();
		console.printSearchResults(spracovaneKraje);
	}

	if (!spracovaneOkresy.isEmpty())
	{
		console.printSearchResultsOkresyTitle();
		console.printSearchResults(spracovaneOkresy);
	}

	if (!spracovaneObce.isEmpty())
	{
		console.printSearchResultsObceTitle();
		console.printSearchResults(spracovaneObce);
	}

	console.waitForUserInput();
	console.clear();

	spracovaneObce.clear();
	spracovaneOkresy.clear();
	spracovaneKraje.clear();
}


void App::hierarchyLoop()
{
	// Iterator hierarchie uzemnych jednotiek
	auto iterator = hierarchy.beginLevel();

	// Cyklus programu
	while (true)
	{
		console.hierarchyMenu(iterator);
		console.input(userInput);
		console.clear();

		if (userInput == "0")
		{
			iterator.goToParent();
		}
		if (userInput == "*")
		{
			runAlgorithm(hierarchy, iterator);
		}
		else if (userInput == "x")
		{
			console.clear();
			break;
		}
		else if (atoi(userInput.c_str()) > 0)
		{
			iterator.goToSon(atoi(userInput.c_str()) - 1);
		}

	}
}


void App::runAlgorithm(ds::amt::MultiWayExplicitHierarchy<DataUnit*>& hierarchy,
	const ds::amt::Hierarchy<ds::amt::MultiWayExplicitHierarchyBlock<DataUnit*>>::LevelAccessHierarchyIterator& iterator)
{
	console.filterPredicatesMenu();

	std::string selectedOption;
	console.input(selectedOption);
	console.clear();

	console.filterAskForSearchArgument(selectedOption);
	std::string searchPhrase;
	console.input(searchPhrase);
	console.clear();

	matchingDataUnits.clear();

	filterAlgorithm.filterItemsUsingPredicateIndex<DataUnit*>(
		iterator,
		hierarchy.endLevel(),
		selectedOption,
		searchPhrase,
		[&](DataUnit* dataUnit)
		{
			matchingDataUnits.insertLast().data_ = dataUnit;
		}
	);


	if (matchingDataUnits.isEmpty())
	{
		console.noResults();
	}
	else
	{
		console.printSearchResultsGeneralTitle();
		console.printSearchResults(matchingDataUnits);
	}

	console.waitForUserInputAskForSorting();
	console.input(selectedOption);

	if (selectedOption == "&")
	{
		console.clear();
		sortingLoop();
	}
	
	console.clear();
}


void App::tableLoop()
{
	console.selectTable();
	std::string type;
	console.input(type);
	console.clear();

	console.enterKey();
	std::string key;
	console.input(userInput);
	console.clear();

	if (type == "kraje")
	{
		ds::adt::ImplicitList<Kraj*>** kraje = nullptr;
		krajeTable->tryFind(userInput, kraje);

		if (kraje == nullptr)
		{
			console.noResultKraj();
		}
		else
		{
			console.printSearchResultsDetails(**kraje);
		}
	}
	else if (type == "okresy")
	{
		ds::adt::ImplicitList<Okres*>** okresy = nullptr;
		okresyTable->tryFind(userInput, okresy);

		if (okresy == nullptr)
		{
			console.noResultOkres();
		}
		else
		{
			console.printSearchResultsDetails(**okresy);
		}
	}
	else
	{
		ds::adt::ImplicitList<Obec*>** obce = nullptr;
		obceTable->tryFind(userInput, obce);

		if (obce == nullptr)
		{
			console.noResultObec();
		}
		else
		{
			console.printSearchResultsDetails(**obce);
		}
	}

	console.waitForUserInput();
	console.clear();
}


void App::sortingLoop()
{
	console.sortingComparatorsMenu();
	console.input(userInput);
	console.clear();

	sortingAlgorithm.sortItemsUsingComparatorIndex(matchingDataUnits, userInput);

	console.printSearchResults(matchingDataUnits);

	console.waitForUserInput();
}


