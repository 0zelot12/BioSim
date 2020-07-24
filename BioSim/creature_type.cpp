#include "creature_type.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: creature_type
//
//
/////////////////////////////////////////////////////////////

creature_type::creature_type(const std::string& name, unsigned int staerke, unsigned int geschwindigkeit, unsigned int lebensdauer, const std::string& eigenschaften, const std::string& bild_pfad)
	:
	name_(name),
	eigenschaften_(eigenschaften),
	bild_pfad_(bild_pfad),
	staerke_(staerke),
	geschwindigkeit_(geschwindigkeit),
	lebensdauer_(lebensdauer)

{
	attributes::set_eigenschaften(eigenschaften, eigenschaften_list_);
}

const std::vector<std::string> ATTRIBUTE_NAMES = { "Name", "Staerke", "Geschwindigkeit", "Lebensdauer", "Eigenschaften", "Bild" };

std::string creature_type::name() const
{
	return name_;
}

std::string creature_type::bild_pfad() const
{
	return bild_pfad_;
}

unsigned int creature_type::staerke() const
{
	return staerke_;
}

unsigned int creature_type::geschwindigkeit() const
{
	return geschwindigkeit_;
}

unsigned int creature_type::lebensdauer() const
{
	return lebensdauer_;
}

std::vector<properties> creature_type::eigenschaften_list() const
{
	return eigenschaften_list_;
}

std::vector<std::shared_ptr<creature_type>> creature_type::load(const std::string& path)
{
	try
	{
		std::vector<std::shared_ptr<creature_type>> creatures_types;

		std::ifstream file_stream;
		std::string line, word;

		unsigned int lines_read = 0, lines_with_error = 0;

		file_stream.open(path);

		if (file_stream.is_open())
		{
			while (getline(file_stream, line))
			{
				lines_read++;

				std::vector<std::string> attribute_string_list;
				std::stringstream str_stream(line);

				std::cout << "Reading line: " << lines_read << std::endl;

				// check also if line contains only spaces
				if (!line.empty() && line.find_first_not_of(' ') != std::string::npos)
				{
					while (getline(str_stream, word, ','))
					{
						unsigned int attr_idx = 0;

						if (word == "")
						{
							std::cout << "-> [ERROR]: Empty cell found, but expected " << ATTRIBUTE_NAMES.at(attr_idx) << std::endl;
							lines_with_error++;
							attr_idx++;
							continue;
						}

						attribute_string_list.push_back(word);
						attr_idx++;
					}

					// skip line, if at least one attribute is invalid
					if (!attributes::check_attributes(attribute_string_list))
					{
						lines_with_error++;
						continue;
					}

					try
					{
						creatures_types.push_back(std::make_shared<creature_type>(	attribute_string_list[0], 
															(unsigned int)std::stoi(attribute_string_list[1]),
															(unsigned int)std::stoi(attribute_string_list[2]),
															(unsigned int)std::stoi(attribute_string_list[3]), 
																					attribute_string_list[4], 
																					attribute_string_list[5])
						);
					}

					catch (const std::exception& e)
					{
						std::cout << "-> [ERROR]: Couldn't instantiate creature_type object: " << e.what() << std::endl;
					}
				}
				else
				{
					std::cout << "-> [ERROR]: Empty line detected" << std::endl;
					lines_with_error++;
				}
			}

			file_stream.close();

			creature_type::print_stats(lines_read, lines_with_error);
		}
		else
		{
			throw file_not_found();
		}

		return creatures_types;
	}
	catch (const std::exception& e)
	{
		throw file_not_found();
	}
}

void creature_type::print_stats(int lines_read, int lines_read_with_error)
{
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Lines read successfull: " << lines_read - lines_read_with_error << std::endl;
	std::cout << "Lines read with errors: " << lines_read_with_error << std::endl;
	std::cout << "Lines read in total: " << lines_read << std::endl;
	std::cout << "----------------------------------" << std::endl;
}