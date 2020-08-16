#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <memory>

#include "attributes.hpp"
#include "common_exceptions.hpp"

/**
*************************************************************************
* @class creature_type
*
* Instances of this class represent a type of a creature, with their properties
* Types are loaded from a csv-File and then verified
*************************************************************************/
class creature_type
{

public:

	// Loads types from a comma-seperated list of values and create vector of shared pointer to use them further on 
	static std::vector<std::shared_ptr<creature_type>> load(const std::string& path);

	creature_type(const std::string& name, unsigned int strength, unsigned int speed,
		unsigned int life_span, const std::string& properties, const std::string& image_path);

	std::string	name() const;
	std::string	image_path()	const;
	unsigned int strength() const;
	unsigned int speed() const;
	unsigned int life_span() const;
	std::vector<PROPERTIES> property_list() const;

private:

	std::string m_name, m_properties, m_image_path;
	unsigned int m_strength, m_speed, m_life_span;

	// Vector containing the converted content of eigentschaften
	std::vector<PROPERTIES> m_properties_list;

	// Writes number of successful/unsucessful read lines on the console 
	static void print_stats(int lines_read, int lines_read_with_error);
};