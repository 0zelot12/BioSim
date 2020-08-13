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