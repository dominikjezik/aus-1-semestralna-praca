#include "CsvLoader.h"

void CsvLoader::load(const std::string& filePath, const std::function<void(std::string[])>& lineAction) const
{
	// stream na nacitanie suboru
	std::fstream file(filePath, std::ios::in);

	// aktualny riadok
	std::string line;

	if (file.is_open())
	{
		// vyber najskor hlavicku s ktorou nic nerob
		getline(file, line);

		// vyberaj riadky, spracuj ich do pola segmentov a spusti akciu pre kazdy spracovany riadok
		while (getline(file, line))
		{
			// ziska dynamicky alokovane pole segmentov
			std::string* segments = parseLine(line);

			// pre dany riadok spusti zadanu funkciu
			lineAction(segments);

			// dealokuje pole segmentov
			delete[] segments;
		}
	}
	else
	{
		throw std::runtime_error("Nepodarilo sa otvorit subor " + filePath);
	}
}


std::string* CsvLoader::parseLine(const std::string& line) const
{
	// stream na nacitanie segmentov
	std::stringstream streamLine(line);

	// aktualny segment
	std::string segment;

	// nacitane segmenty
	std::string* segments = new std::string[6];

	// Nacitavaj segmenty pokial existuju respektive max 6 segmentov
	int i = 0;
	while (i < 6 && std::getline(streamLine, segment, ';'))
	{
		segments[i] = segment;
		++i;
	}

	return segments;
}
