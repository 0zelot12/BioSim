#pragma once

#include "creature.hpp"

/**
*************************************************************************
* @enum class TERRAIN_TYPE
*
* Enum type for the different terrain types
*************************************************************************/

enum class TERRAIN_TYPE
{
	DEEP_WATER,
	SHALLOW_WATER,
	SAND,
	EARTH,
	STONE,
	SNOW
};

struct POINT_2D
{
	int x, y;
};

class tile
{
public:

	// Type of tile
	TERRAIN_TYPE m_terrain_type;

	// Index to identify tiles on map
	unsigned int m_tile_map_idx;

	// All creatures on this tile 
	std::vector<std::shared_ptr<creature>> m_creatures_on_tile;

	// Pointer to last simulation tile in a path of the A*-algorithm
	tile* m_predecessor = nullptr;

	// Indicats whether a tile is located on the edge of the map
	bool m_is_left_edge = false, m_is_right_edge = false;

	/* Distances needed for A*-algorithm */
	float m_g_distance = 0, m_f_distance = 0, m_h_distance = 0;

	// Position on map in 2D coordinates
	POINT_2D m_pos;

};

