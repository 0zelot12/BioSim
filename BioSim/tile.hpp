#pragma once

#include "world.hpp"

class tile
{
public:
	const TERRAIN_TYPE& terrain_type;

	void set_creature	(const std::shared_ptr<creature>& creature);
	void remove_creature(const std::shared_ptr<creature>& creature);

private:
	std::vector<std::shared_ptr<creature>> containing_creatures;
};
