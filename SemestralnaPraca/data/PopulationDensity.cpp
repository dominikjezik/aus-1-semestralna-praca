#include "PopulationDensity.h"

PopulationDensity::PopulationDensity(const std::string& code, const float value)
{
	this->code_ = code;
	this->value_ = value;
}


bool PopulationDensity::operator==(const PopulationDensity& other) const
{
	return
		other.value() == this->value_ &&
		other.code() == this->code_;
}


std::string PopulationDensity::code() const
{
	return this->code_;
}


float PopulationDensity::value() const
{
	return this->value_;
}
