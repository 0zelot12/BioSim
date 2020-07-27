#include "creature.hpp"

creature::creature(unsigned int strength, unsigned int life_span, int position, std::string name, std::vector<properties> properties, QPixmap* creature_image, bool is_land_creature)
	:
	m_strength	(strength),
	m_speed		(strength),
	m_life_span	(life_span),
	m_name		(name),
	m_properties(properties),
	m_position	(position),
	m_creature_image(creature_image),
	m_is_land_creature(is_land_creature)
{

}
