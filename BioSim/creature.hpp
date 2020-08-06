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
	creature(	unsigned int strength, 
				unsigned int life_span, 
				int position, 
				std::string name, 
				std::vector<properties> properties, 
				QPixmap* creature_image, 
				bool is_land_creature);

	unsigned int				m_strength, m_speed, m_life_span;
	int							m_current_position;
	std::string					m_name;
	std::vector<properties>		m_properties;
	bool						m_is_land_creature;
	/* Image of the creature */
	QPixmap*					m_creature_image;
};