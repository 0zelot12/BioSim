#include "bio_sim_model.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: bio_sim_model
//
//
/////////////////////////////////////////////////////////////

bio_sim_model::bio_sim_model(const std::string& path_to_creature_table)
	:
	m_world(WORLD_WIDTH_TILES, WORLD_HEIGHT_TILES)
{
	m_creature_types = creature_type::load(path_to_creature_table);
}

std::vector<QSimulationTile> bio_sim_model::test_pathfinding()
{
	a_star alg(this->m_world);
	return alg.calc_path(m_world.creature_map.at(0), m_world.tile_map.at(500));
}
