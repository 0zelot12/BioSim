#include "bio_sim_model.hpp"

bio_sim_model::bio_sim_model(const std::string& path_to_creature_table)
	:
	m_world(WORLD_WIDTH, WORLD_HEIGHT)
{
	m_creature_types = creature_type::load(path_to_creature_table);
}

void bio_sim_model::test_pathfinding()
{
	a_star alg(this->m_world);
	alg.calc_path(m_world.creatures_total.at(0), m_world.tile_map.at((m_world.creatures_total.at(0)->m_current_position) - 128));
}
