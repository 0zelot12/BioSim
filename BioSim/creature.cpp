#include "creature.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: creature
//
//
/////////////////////////////////////////////////////////////

creature::creature(unsigned int strength, unsigned int life_span, int position, std::string name, std::vector<PROPERTIES> properties, QPixmap* creature_image)
	:
	m_strength			(strength			),
	m_speed				(strength			),
	m_life_span			(life_span			),
	m_name				(name				),
	m_properties		(properties			),
	m_current_position	(position			),
	m_creature_image	(creature_image		)
{

}

PROPERTIES creature::terrain_type()
{
	for (auto& property : m_properties)
	{
		if (property == PROPERTIES::LANDBEWOHNER)
			return PROPERTIES::LANDBEWOHNER;
	}

	return PROPERTIES::WASSERBEWOHNER;
}
