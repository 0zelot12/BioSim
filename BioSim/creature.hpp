#pragma once

#include "creature_type.hpp"
#include "image.hpp"

#include <vector>
#include <string>

class creature
{
public:
	creature(const creature_type& type, int position);
private:
	unsigned int				m_strength, m_speed, m_life_span;
	int							m_position;
	std::string					m_name;
	std::vector<properties>		m_properties;
};