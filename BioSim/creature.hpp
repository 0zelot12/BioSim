#pragma once

#include "creature_type.hpp"
#include "image.hpp"

#include <vector>
#include <string>

/**
*************************************************************************
* @enum STATE
*
* Represents state of one animal in simualation
*************************************************************************/
enum class STATE
{
	WAIT,
	GROW,
	DEAD,
	MATE,
	HUNT,
	REST,
	ATTACK,
	WANDER,
	NEW
};


/**
*************************************************************************
* @struct TYPES
*
* Types of properties as data object for simpler access
*************************************************************************/
struct TYPES
{
	PROPERTIES terrain_type, entity_type, prey_type;
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

	// Returns wether the creature is an animal or a plant
	PROPERTIES entity_type();

	// Returns wether the creature eats meat or plants
	PROPERTIES prey_type();

	// Current state in the simualation
	STATE m_current_state = STATE::NEW;

	// Fills values of m_types accordings to m_properties
	void set_types();

	// Check whether the two creatures are from the same type
	static bool have_same_type(const std::shared_ptr<creature>& creature_1, const std::shared_ptr<creature>& creature_2);
	
	// Current target that creature trys to attack, null in case of no target
	std::shared_ptr<creature> m_currrent_target;

	// Vector containing the (remaining) indicees to currrent_target
	std::vector<int> m_path_to_current_target;

	std::string m_name;

	std::vector<PROPERTIES> m_properties;

	int m_strength, m_speed, m_health;

	int m_steps_since_last_rest = 0, m_steps_since_last_replication = 0, m_steps_since_death = 0;

	double m_position_x, m_position_y;
	
	TYPES m_types;
};