#include "creature.hpp"

creature::creature(const creature_type& type, int position)
	:
	m_strength	(type.staerke()				),
	m_speed		(type.geschwindigkeit()		),
	m_life_span	(type.lebensdauer()			),
	m_name		(type.name()				),
	m_properties(type.eigenschaften_list()	),
	m_position	(position					)
{

}
