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

class bio_sim_model
{
public:

	bio_sim_model(const std::string& path_to_creature_table);
	std::vector<std::shared_ptr<creature_type>> m_creature_types;
	world m_world;

	const static int WORLD_HEIGHT = 128;
	const static int WORLD_WIDTH = 128;
};