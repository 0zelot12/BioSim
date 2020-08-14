#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <memory>
#include <regex>
#include <filesystem>
#include <map>
#include <algorithm>
#include <string>

/**
*************************************************************************
* @class entity_type
*
* Properties of a creature_type
*************************************************************************/
enum class PROPERTIES
{
	PFLANZE, 
	WASSERBEWOHNER, 
	LANDBEWOHNER, 
	TIER, 
	PFLANZENFRESSER, 
	FLEISCHFRESSER 
};

/**
*************************************************************************
* @class entity_type
*
* Class contains all methods regarding validating 
* and setting attributes of an creature_type
*************************************************************************/
class attributes
{
public:
	/* Checks if all given attributes are valid. Returns true if alle attributes are valid, false otherwise */
	static bool check_attributes		(const std::vector<std::string>& attributes);

	/* Converts a string containing Eigenschaften of a creature_type to a list of properties elements */
	static void set_eigenschaften		(const std::string& eigenschaften_string, std::vector<PROPERTIES>& property_list);

	/* Gets string representatiopn of a given property */
	static std::string property_to_str	(PROPERTIES prop);

private:

	static bool is_valid_name				(const std::string& input	);
	static bool is_valid_staerke			(const std::string& input	);
	static bool is_valid_geschwindigkeit	(const std::string& input	);
	static bool is_null_or_empty			(const std::string& input	);
	static bool is_valid_lebensdauer		(const std::string& input	);
	static bool is_valid_eingenschaften		(const std::string& input	);
	static bool is_valid_eigenschaft		(const std::string& input	);
	static bool is_valid_path				(const std::string& path	);
	static bool contains_numbers			(const std::string& input	);
	static bool is_valid_int_string			(const std::string& str		);
	static void convert_to_upper			(	   std::string& input	);

	/* Count of expected attributes in row */
	static const unsigned int ATTRIBUTE_COUNT = 6;
};