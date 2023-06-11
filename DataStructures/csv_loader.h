#pragma once

#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

namespace ds::utils
{
	class CsvLoader
	{
	public:
		void load(std::string name, std::function<void(std::string[])> action)
		{
			// stream na nacitanie suboru
			std::fstream file(name, std::ios::in);

			// aktualny riadok
			std::string line;

			if (file.is_open())
			{
				// vyber najskor hlavicku s ktorou nic nerob
				getline(file, line);

				// vyberaj riadky, spracuj ich do pola segmentov a spusti akciu pre kazdy spracovany riadok
				while (getline(file, line))
				{
					std::string* segments = parseLine(line);

					action(segments);

					delete[] segments;
				}
			}
			else
			{
				std::cout << "Subor " << name << " nebolo mozne otvorit.\n";
			}
		}

	private:
		std::string* parseLine(const std::string& line)
		{
			// stream na nacitanie segmentov
			std::stringstream streamLine(line);

			// aktualny segment
			std::string segment;

			// nacitane segmenty
			std::string* segments = new std::string[6];

			// Nacitavaj segmenty pokial existuju respektive max 5 segmentov
			int i = 0;
			while (i < 6 && std::getline(streamLine, segment, ';'))
			{
				segments[i] = segment;
				++i;
			}

			return segments;
		}

	};
}
