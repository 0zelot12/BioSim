#pragma once

#include "creature_type.hpp"
#include "image.hpp"
#include "world.hpp"
#include "a_star.hpp"
#include "QSimulationTile.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <memory>

/**
*************************************************************************
* @class bio_sim_model
*
* Model containing the actual simulation
*************************************************************************/
class bio_sim_model
{
public:

	bio_sim_model(const std::string& path_to_creature_table);

	/* Warum pointer? */
	/* Creature types that the model can handle */
	std::vector<std::shared_ptr<creature_type>> m_creature_types;
	/* World object containing terrain data */
	world										m_world;

	std::vector<QSimulationTile> test_pathfinding();

	/* y dimension of the world in tiles */
	const static int WORLD_HEIGHT_TILES	= 128;
	/* x dimension of the world in tiles */
	const static int WORLD_WIDTH_TILES	= 128;
};