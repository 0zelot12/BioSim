#include "tile.hpp"

void tile::delete_creature_from_tile(std::shared_ptr<creature> creature_to_delete)
{
	int deletion_idx = -1;

	for (int i = 0; i < m_creatures_on_tile.size(); i++)
	{
		if (m_creatures_on_tile[i] == creature_to_delete)
		{
			deletion_idx = i;
		}
	}

	if (deletion_idx != -1)
	{
		m_creatures_on_tile.erase(m_creatures_on_tile.begin() + deletion_idx);
	}
}

void tile::add_creature_to_tile(std::shared_ptr<creature> entity)
{
	m_creatures_on_tile.push_back(entity);
}
