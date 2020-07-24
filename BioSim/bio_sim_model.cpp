#include "bio_sim_model.hpp"

bio_sim_model::bio_sim_model(const std::string& path_to_creature_table)
	:
	m_world(WORLD_WIDTH, WORLD_HEIGHT)
{
	m_creature_types = creature_type::load(path_to_creature_table);
}
