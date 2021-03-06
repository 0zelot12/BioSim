#include "creature.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: creature
//
//
/////////////////////////////////////////////////////////////

creature::creature(unsigned int strength, unsigned int speed, unsigned int life_span, int position, std::string name, std::vector<PROPERTIES> properties, std::shared_ptr<creature_type> type)
	:
	m_strength(strength),
	m_speed(speed),
	m_health(life_span),
	m_name(name),
	m_properties(properties),
	m_current_position(position),
	m_type(type)
{
	set_types();
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

PROPERTIES creature::prey_type()
{
	return m_types.prey_type;
}

PROPERTIES creature::entity_type()
{
	return m_types.entity_type;
}

bool creature::have_same_type(const std::shared_ptr<creature>& creature_1, const std::shared_ptr<creature>& creature_2)
{
	if (creature_1->m_type == creature_2->m_type && creature_1->m_current_state != STATE::DEAD && creature_2->m_current_state != STATE::DEAD) {
		return true;
	}

	return false;
}


void creature::set_types()
{
	for (auto& prop : m_properties)
	{
		switch (prop)
		{
		case PROPERTIES::PFLANZE:
			m_types.entity_type = PROPERTIES::PFLANZE;
			break;
		case PROPERTIES::WASSERBEWOHNER:
			m_types.terrain_type = PROPERTIES::WASSERBEWOHNER;
			break;
		case PROPERTIES::LANDBEWOHNER:
			m_types.terrain_type = PROPERTIES::LANDBEWOHNER;
			break;
		case PROPERTIES::TIER:
			m_types.entity_type = PROPERTIES::TIER;
			break;
		case PROPERTIES::PFLANZENFRESSER:
			m_types.prey_type = PROPERTIES::PFLANZENFRESSER;
			break;
		case PROPERTIES::FLEISCHFRESSER:
			m_types.prey_type = PROPERTIES::FLEISCHFRESSER;
			break;
		default:
			break;
		}
	}
}
