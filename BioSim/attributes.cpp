#include "attributes.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: properties
//
//
/////////////////////////////////////////////////////////////

const std::map<std::string, properties> string_to_property = {
	{"WASSERBEWOHNER", properties::WASSERBEWOHNER},
	{"LANDBEWOHNER", properties::LANDBEWOHNER},
	{"PFLANZE", properties::PFLANZE},
	{"PFLANZENFRESSER", properties::PFLANZENFRESSER},
	{"TIER", properties::TIER},
	{"FLEISCHFRESSER", properties::FLEISCHFRESSER}
};

const std::map<properties, std::string> property_to_string = {
	{properties::WASSERBEWOHNER, "WASSERBEWOHNER"},
	{properties::LANDBEWOHNER, "LANDBEWOHNER"},
	{properties::PFLANZE, "PFLANZE"},
	{ properties::PFLANZENFRESSER, "PFLANZENFRESSER"},
	{properties::TIER, "TIER"},
	{properties::FLEISCHFRESSER, "FLEISCHFRESSER"}
};

/////////////////////////////////////////////////////////////
//
//
//  Class: attributes
//
//
/////////////////////////////////////////////////////////////

bool attributes::check_attributes(const std::vector<std::string>& attributes)
{
	try
	{
		if (attributes.size() > ATTR_COUNT_)
		{
			std::cout << "-> [ERROR]: " << "Too many Attributes found" << std::endl;
			return false;
		}

		if (attributes::is_valid_name(attributes.at(0)) && 
			attributes::is_valid_staerke(attributes.at(1)) &&
			attributes::is_valid_geschwindigkeit(attributes.at(2)) &&
			attributes::is_valid_lebensdauer(attributes.at(3)) &&
			attributes::is_valid_eingenschaften(attributes.at(4)) &&
			attributes::is_valid_path(attributes.at(5))) 
			return true;

		return false;
	}

	// occurs in case the last field was empty
	catch (const std::out_of_range&)
	{
		std::cout << "-> [ERROR]: " << "Invalid vector index - last field was empty" << std::endl;
		return false;
	}

	catch (const std::exception& e)
	{
		std::cout << "-> [EXCEPTION]: " << e.what() << std::endl;
		return false;
	}

	return false;
}

void attributes::set_eigenschaften(const std::string& eigenschaften_string, std::vector<properties>& property_list)
{
	try
	{
		std::istringstream buf(eigenschaften_string);

		// split string by spaces to iterate over it
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end);

		for (auto& str : tokens)
		{
			attributes::convert_to_upper(str);

			for (auto& entry : string_to_property)
			{
				if (entry.first == str)
				{
					properties property = entry.second;
					property_list.push_back(property);
				}
			}
		}
		
		return;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return;
	}
}

bool attributes::is_valid_name(const std::string& input)
{
	if (attributes::is_null_or_empty(input))
	{
		std::cout << "-> [ERROR]: Name is null" << std::endl;
		return false;
	}

	else if (attributes::contains_numbers(input))
	{
		std::cout << "-> [ERROR]: Name contains invalid characters" << std::endl;
		return false;
	}

	return true;
}

bool attributes::is_valid_staerke(const std::string& input)
{
	try
	{
		if (attributes::is_valid_int_string(input))
		{
			int numVal = std::stoi(input);

			if (numVal < 0)
			{
				std::cout << "-> [ERROR]: Staerke has to greater 0" << std::endl;
				return false;
			}
		}

		else
		{
			std::cout << "-> [ERROR]: Staerke is not a valid number" << std::endl;
			return false;
		}
	}
	catch (const std::exception&)
	{
		std::cout << "-> [ERROR]: Staerke is not a valid number" << std::endl;
		return false;
	}

	return true;
}

bool attributes::is_valid_geschwindigkeit(const std::string& geschwindigkeit)
{
	try
	{
		if (attributes::is_valid_int_string(geschwindigkeit))
		{
			int numVal = std::stoi(geschwindigkeit);

			if (numVal < 0)
			{
				std::cout << "-> [ERROR]: Geschwindigkeit has to greater 0" << std::endl;
				return false;
			}
		}

		else
		{
			std::cout << "-> [ERROR]: Geschwindigkeit is not a valid number" << std::endl;
			return false;
		}
	}
	catch (const std::exception&)
	{
		std::cout << "-> [ERROR]: Geschwindigkeit is not a valid number" << std::endl;
		return false;
	}

	return true;
}

bool attributes::is_null_or_empty(const std::string& string)
{
	return string.empty() == true || string.find_first_not_of(' ') == std::string::npos;
}

bool attributes::contains_numbers(const std::string& input)
{
	for (int i = 0; i < input.size(); i++)
	{
		// check if char is a digit by its ASCII representation
		if ((int)(input[i]) < 65 || ((int)(input[i]) > 90 && (int)(input[i]) < 97) || (int)(input[i]) > 122)
		{
			return true;
		}
	}
	return false;
}

bool attributes::is_valid_lebensdauer(const std::string& input)
{
	try
	{
		if (attributes::is_valid_int_string(input))
		{
			int numVal = std::stoi(input);

			if (numVal < 0)
			{
				std::cout << "-> [ERROR]: Lebensdauer has to greater 0" << std::endl;
				return false;
			}
		}

		else
		{
			std::cout << "-> [ERROR]: Lebensdauer is not a valid number" << std::endl;
			return false;
		}
	}
	catch (const std::exception&)
	{
		std::cout << "-> [ERROR]: Lebensdauer is not a valid number" << std::endl;
		return false;
	}

	return true;
}

bool attributes::is_valid_eingenschaften(const std::string& input)
{
	try
	{
		if (attributes::is_null_or_empty(input))
		{
			std::cout << "-> [ERROR]: Eigenschaften is empty" << std::endl;
			return false;
		}

		// matches everything except letters, digits and underscores
		std::regex word_expr("\\W");
		std::smatch word_matches;

		// matches spaces
		std::regex space_expr("\\s");
		std::smatch space_matches;

		// split string by spaces to iterate over it
		std::istringstream buf(input);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end);

		// get number of spaces in string
		std::ptrdiff_t const match_count(std::distance(std::sregex_iterator(input.begin(), input.end(), space_expr), std::sregex_iterator()));
		  
		// only one space between two words
		if ((size_t)match_count > tokens.size() + 1)
		{
			std::cout << "-> [ERROR]: Too many spaces found in Eigenschaften" << std::endl;
			return false;
		}

		for (auto& str : tokens)
		{
			if (!attributes::is_valid_eigenschaft(str))
			{
				std::cout << "-> [ERROR]: Invalid chararcter found in Eigenschaften" << std::endl;
				return false; 
			}

			std::regex_search(str, word_matches, word_expr);

			if (!word_matches.empty())
			{
				return false;
			}
		}

		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	return false;
}

bool attributes::is_valid_eigenschaft(const std::string& input)
{
	if (attributes::is_null_or_empty(input) || attributes::contains_numbers(input))
	{
		return false;
	}

	return true;
}

bool attributes::is_valid_path(const std::string& path)
{
	if (attributes::is_null_or_empty(path))
	{
		std::cout << "-> [ERROR]: Path is empty" << std::endl;
		return false;
	}

	std::filesystem::path p = path;

	// matches everything except letters, digits and underscores
	std::regex reg(".?.?(/[^a-zA-Z0-9_.-])+"); 
	std::smatch matches;

	std::istringstream buf(path);
	std::istream_iterator<std::string> beg(buf), end;
	std::vector<std::string> symbols(beg, end);

	std::regex_search(path, matches, reg);
	if (!matches.empty())
	{
		std::cout << "-> [ERROR]: Invalid path" << std::endl;
		return false;
	}

	if (p.is_relative() && !p.empty())
	{
		return true;
	}

	std::cout << "-> [ERROR]: Invalid path" << std::endl;
	return false;
}

bool attributes::is_valid_int_string(const std::string& str)
{
	try
	{
		size_t lastChar;
		int intValue = std::stoi(str, &lastChar);
		return lastChar == str.size();
	}

	catch (const std::exception&)
	{
		return false;
	}
}

std::string attributes::property_to_str(properties prop)
{
	return property_to_string.find(prop)->second;
}

void attributes::convert_to_upper(std::string& input)
{
	std::transform(input.begin(), input.end(), input.begin(), ::toupper);
}