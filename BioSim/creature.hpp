#pragma once

#include "creature_type.hpp"
#include "image.hpp"

#include <vector>
#include <string>

enum class STATE
{
	WAIT,
	GROW,
	DEAD,
	NEW
};

struct TYPES
{
	PROPERTIES terrain_type, prey_type, entity_type;
};

/**
*************************************************************************
* @class creature
*
* Class holds the actual instance of a creature_type
*************************************************************************/
class creature
{
public:

	creature(unsigned int strength, unsigned int speed, unsigned int life_span, int	position, std::string name, std::vector<PROPERTIES>	properties, std::shared_ptr<creature_type> type);

	// Current position on the map
	int m_current_position;

	// Pointer to original type
	std::shared_ptr<creature_type> m_type;

	// Returns wether the creature is a land or water creature
	PROPERTIES terrain_type();

	// Returns wether the creature is a land or water creature
	PROPERTIES prey_type();

	// Returns wether the creature is a land or water creature
	PROPERTIES entity_type();

	// Current state in the simualation
	STATE m_current_state = STATE::NEW;

	// Check whether the two creatures are from the same type
	static bool have_same_type(const std::shared_ptr<creature>& creature_1, const std::shared_ptr<creature>& creature_2);


	std::string m_name;
	std::vector<PROPERTIES> m_properties;
	unsigned int m_strength, m_speed, m_life_span;

private:

	// Sets the types of the creature for covenience
	void set_types();

	// Type struct
	TYPES m_types;
};