#pragma once

#include "creature_type.hpp"
#include "image.hpp"

#include <vector>
#include <string>

class QSimulationTile;

/**
*************************************************************************
* @class creature
*
* Class holds the actual instance of a creature_type
*************************************************************************/

class creature
{
public:

	creature(	unsigned int			strength, 
				unsigned int			life_span, 
				int						position, 
				std::string				name, 
				std::vector<PROPERTIES>	properties, 
				QPixmap*				creature_image);


	unsigned int m_strength, m_speed, m_life_span;
	int m_current_position;
	std::string m_name;
	std::vector<PROPERTIES> m_properties;
	QPixmap* m_creature_image;
	std::shared_ptr<creature_type> m_type;

	// Returns wether the creature is a land or water creature
	PROPERTIES terrain_type();
	
};