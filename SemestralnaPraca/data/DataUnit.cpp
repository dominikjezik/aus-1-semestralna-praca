#include "DataUnit.h"

DataUnit::DataUnit(std::string const& official_title)
{
	this->official_title_ = official_title;
}


bool DataUnit::operator==(const DataUnit& other) const
{
	return
		this->sort_number_ == other.sort_number() &&
		this->code_ == other.code_ &&
		this->official_title_ == other.official_title_ &&
		this->medium_title_ == other.medium_title_ &&
		this->short_title_ == other.short_title_ &&
		this->note_ == other.note_;
}


DataUnit& DataUnit::operator=(const DataUnit& other)
{
	this->sort_number_ = other.sort_number_;
	this->code_ = other.code_;
	this->official_title_ = other.official_title_;
	this->medium_title_ = other.medium_title_;
	this->short_title_ = other.short_title_;
	this->note_ = other.note_;
	return *this;
}


void DataUnit::parseData(std::string line[])
{
	this->sort_number_ = stoi(line[0]);
	this->code_ = line[1];
	this->official_title_ = line[2];
	this->medium_title_ = line[3];
	this->short_title_ = line[4];
	this->note_ = line[5];
}


void DataUnit::print() const
{
	std::cout << std::setw(4) << sort_number_ << " | " << std::setw(12) << code_ << " | " << std::setw(7) << population_density_ << " | " << official_title_ << std::endl;
}


void DataUnit::printDetails() const
{
	std::cout << "  Sort number: " << sort_number_ << std::endl
		<< "  Code: " << code_ << std::endl
		<< "  Official Title: " << official_title_ << std::endl
		<< "  Medium Title: " << medium_title_ << std::endl
		<< "  Short Title: " << short_title_ << std::endl
		<< "  Note: " << note_ << std::endl;
}


// ----- Getters -----
std::string DataUnit::official_title() const
{
	return this->official_title_;
}

std::string DataUnit::medium_title() const
{
	return this->medium_title_;
}

std::string DataUnit::short_title() const
{
	return this->short_title_;
}

std::string DataUnit::code() const
{
	return this->code_;
}

std::string DataUnit::note() const
{
	return this->note_;
}

int DataUnit::sort_number() const
{
	return this->sort_number_;
}

float DataUnit::population_density() const
{
	return this->population_density_;
}


int DataUnit::vowels_count()
{
	if (vowels_count_ != - 1)
	{
		return vowels_count_;
	}

	vowels_count_ = 0;

	char samohlasky[] = {
		'a', 'ä', 'e', 'i', 'o', 'u', 'y', 'á', 'é', 'í', 'ó', 'ú', 'ý',
		'A', 'Ä', 'E', 'I', 'O', 'U', 'Y', 'Á', 'É', 'Í', 'Ó', 'U', 'Ý'
	};

	for (size_t i = 0; i < official_title_.length(); ++i)
	{
		char a = official_title_.at(i);

		for (size_t j = 0; j < sizeof(samohlasky); ++j)
		{
			if (a == samohlasky[j])
			{
				++vowels_count_;
				break;
			}
		}
	}

	return vowels_count_;
}


// --------------------
AggregatedDataUnit::AggregatedDataUnit(const std::string& official_title)
	:DataUnit(official_title)
{
}


void AggregatedDataUnit::printDetails() const
{
	std::cout << "   Sort number: " << sort_number() << std::endl
		<< "   Code: " << code() << std::endl
		<< "   Official Title: " << official_title() << std::endl
		<< "   Medium Title: " << medium_title() << std::endl
		<< "   Short Title: " << short_title() << std::endl
		<< "   Note: " << note() << std::endl
		<< "   Priemerna hustota obyvatelstva obce: " << population_density_ << std::endl;
}


void AggregatedDataUnit::printAggregatedData() const
{
	std::cout << "   Priemerna hustota obyvatelstva obce: " << population_density_ << std::endl;
}


void AggregatedDataUnit::addPopulationDensity(float pd)
{
	++count_of_obce_;
	this->sum_population_density_ += pd;
	this->population_density_ = this->sum_population_density_ / count_of_obce_;
}


// --------------------
void Obec::population_density(const float pd)
{
	this->population_density_ = pd;
}


void Obec::printDetails() const
{
	std::cout << "   Sort number: " << sort_number() << std::endl
		<< "   Code: " << code() << std::endl
		<< "   Official Title: " << official_title() << std::endl
		<< "   Medium Title: " << medium_title() << std::endl
		<< "   Short Title: " << short_title() << std::endl
		<< "   Note: " << note() << std::endl
		<< "   Hustota obyvatelstva: " << population_density_ << std::endl;
}
