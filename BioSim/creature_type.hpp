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
private:
	std::string		name_, eigenschaften_, bild_pfad_;
	unsigned int	staerke_, geschwindigkeit_, lebensdauer_;

	/* Vector containing the converted content of eigentschaften_  */
	std::vector<properties> eigenschaften_list_;

	/* Writes number of successful/unsucessful read lines on the console */
	static void print_stats(int lines_read, int lines_read_with_error);

public:
	/* Loads types from a comma-seperated list of values and create vector of shared pointer to use them further on */
	static std::vector<std::shared_ptr<creature_type>> load(const std::string& path);

	creature_type(const std::string& name, unsigned int staerke, unsigned int geschwindigkeit,
		unsigned int lebensdauer, const std::string& eigenschaften, const std::string& bild_pfad);

	std::string name() const;
	std::string bild_pfad() const;
	unsigned int staerke() const;
	unsigned int geschwindigkeit() const;
	unsigned int lebensdauer() const;
	std::vector<properties> eigenschaften_list() const;


	~creature_type() = default;
	creature_type(const creature_type&) = default;
	creature_type& operator=(const creature_type&) = default;
};