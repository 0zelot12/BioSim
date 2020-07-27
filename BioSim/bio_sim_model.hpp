#pragma once

#include "creature_type.hpp"
#include "image.hpp"
#include "world.hpp"

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

	/* Creature types that the model can handle */
	std::vector<std::shared_ptr<creature_type>> m_creature_types;
	/* World object containing terrain data */
	world										m_world;

	/* y dimension of the world in tiles */
	const static int WORLD_HEIGHT	= 128;
	/* x dimension of the world in tiles */
	const static int WORLD_WIDTH	= 128;
};