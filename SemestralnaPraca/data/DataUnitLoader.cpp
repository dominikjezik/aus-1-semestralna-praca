#include "DataUnitLoader.h"

DataUnitLoader::DataUnitLoader(ds::amt::MultiWayExplicitHierarchy<DataUnit*>* hierarchy, const std::string rootTitle,
	const std::string krajeFilePath, const std::string okresyFilePath, const std::string obceFilePath, const std::string hustotaFilePath)
{
	this->hierarchy_ = hierarchy;
	this->rootTitle_ = rootTitle;
	this->krajeFilePath_ = krajeFilePath;
	this->okresyFilePath_ = okresyFilePath;
	this->obceFilePath_ = obceFilePath;
	this->hustotaFilePath_ = hustotaFilePath;
}


void DataUnitLoader::insertRoot() const
{
	const auto root = &hierarchy_->emplaceRoot();
	root->data_ = new AggregatedDataUnit(this->rootTitle_);
}


void DataUnitLoader::loadKraje(std::function<void(Kraj*)> loadCallback) const
{
	const auto root = hierarchy_->accessRoot();

	// Pozicia syna
	int i = 0;

	// V kazdom kroku nacitavania vlozi kraj na i-te miesto syna korena hierarchie.
	csvLoader_.load(this->krajeFilePath_, [&](std::string* line)
	{
		Kraj* kraj = new Kraj();
		kraj->parseData(line);
		loadCallback(kraj);

		// Chceme adresu ulozit do pointra, ina sa to same dealokuje na konci tohto bloku kodu.
		const auto son = &hierarchy_->emplaceSon(*root, i++);
		son->data_ = kraj;
	});
}


void DataUnitLoader::loadOkresy(std::function<void(Okres*)> loadCallback) const
{
	const auto root = hierarchy_->accessRoot();

	// Ziskaj iterator krajov
	auto iteratorKrajov = root->sons_->begin();

	// Pozicia vkladaneho syna
	int i = 0;

	csvLoader_.load(this->okresyFilePath_, [&](std::string* line)
	{
		Okres* okres = new Okres();
		okres->parseData(line);
		loadCallback(okres);

		// Note kraja, na ktory ukazuje iterator krajov.
		const std::string subNoteKraj = (*iteratorKrajov)->data_->note().substr(5, 5);

		// Kod, ktory obsahuje aktualny okres (kod kraja ku ktoremu okres patri).
		const std::string subCodeOkres = okres->code().substr(0, 5);

		// Ak aktualny okres nepatri do kraja, na ktory ukazuje iterator krajov,
		// tak iterator krajov posun na dalsi kraj. Ak je kod kraja "*****",
		// narazili sme na posledny kraj kde vlozime polozky "Zahranicie".
		if (subNoteKraj != subCodeOkres && subNoteKraj != "*****")
		{
			++iteratorKrajov;
			i = 0;
		}

		// Vlozime noveho syna medzi synov kraja (na i-tu poziciu), na ktory ukazuje interator krajov.
		const auto son = &hierarchy_->emplaceSon(*(*iteratorKrajov), i++);

		// Do noveho syna vlozime okres.
		son->data_ = okres;
	});
}


void DataUnitLoader::loadObce(std::function<void(Obec*)> loadCallback)
{
	// Nacitaj informacie o hustote obyvatelstva v obciach do fronty.
	this->loadPopulationDensities();

	const auto root = hierarchy_->accessRoot();

	// Ziskaj iterator krajov
	auto iteratorKrajov = root->sons_->begin();

	// Ziskaj iterator okresov (okresy prveho kraju)
	auto iteratorOkresov = (*iteratorKrajov)->sons_->begin();

	// Pozicia vkladaneho syna
	int i = 0;

	csvLoader_.load(this->obceFilePath_, [&](std::string* line)
	{
		Obec* obec = new Obec();
		obec->parseData(line);
		loadCallback(obec);

		// Kod okresu, na ktory ukazuje iterator okresov.
		const std::string& codeOkres = (*iteratorOkresov)->data_->code();

		// Kod aktualnej obce (kod okresu, ku ktoremu obec patri)
		const std::string& subCodeObec = obec->code().substr(0, 6);

		// Ak aktualna obec nepatri do okresu na ktory ukazuje iterator okresov,
		// posun iterator okresov dopredu na dalsi okres.
		if (codeOkres != subCodeObec)
		{
			++iteratorOkresov;
			i = 0;

			// Ak sa iterator okresov dostal na koniec (v kraji na ktory ukazuje iterator krajov nie je viac okresov)
			// tak posun interator krajov na dalsi kraj a interator okresov nastav na prvy okres noveho kraju
			if (iteratorOkresov == (*iteratorKrajov)->sons_->end())
			{
				++iteratorKrajov;
				iteratorOkresov = (*iteratorKrajov)->sons_->begin();
			}
		}

		// Vlozime noveho syna medzi synov okresu (na i-tu poziciu), na ktory ukazuje interator okresov.
		const auto son = &hierarchy_->emplaceSon(*(*iteratorOkresov), i++);

		// Do noveho syna vlozime obec.
		son->data_ = obec;

		// Ak front hustoty obyvatelstva nie je prazdny a jeho vrchol patri aktualnej obci,
		// tak vyber hustotu obyvatelstva a prirad ju aktualnej obci a tiez tuto
		// pridaj okresu a kraju pod ktory patri dana obec a tie do korena.
		if (!populationDensities.isEmpty() && populationDensities.peek()->code() == obec->code())
		{
			auto pd = populationDensities.pop();

			obec->population_density(pd->value());

			const auto okres = dynamic_cast<Okres*>((*iteratorOkresov)->data_);
			const auto kraj = dynamic_cast<Kraj*>((*iteratorKrajov)->data_);
			const auto rootAgg = dynamic_cast<AggregatedDataUnit*>(root->data_);
			okres->addPopulationDensity(pd->value());
			kraj->addPopulationDensity(pd->value());
			rootAgg->addPopulationDensity(pd->value());

			// Dealokuj dynamicky alokovanu hustotu obyvatelstva
			delete pd;
		}

	});
}


void DataUnitLoader::loadPopulationDensities()
{
	csvLoader_.load(this->hustotaFilePath_, [&](const std::string* line)
	{
		const auto pd = new PopulationDensity(line[0], std::stof(line[1]));
		populationDensities.push(pd);
	});
}
