#pragma once

#include <map>
#include <vector>
#include <memory>
#include <algorithm>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>

#include "image.hpp"
#include "FastNoise.h"
#include "creature.hpp"
#include "QSimulationTile.hpp"

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

/**
*************************************************************************
* @class world
*
* Class holding the content of the simulation
*************************************************************************/
class world
{
public:

	// Contains the type of each tile 
	std::vector<TERRAIN_TYPE> m_terrain_map;

	// Contains all tiles */
	std::vector<QSimulationTile*> m_tile_map;

	// Contains pointers to all creature instances placed on the map 
	std::vector<std::shared_ptr<creature>> m_creature_map;

	//Height and width of the world in number of tiles 
	unsigned int m_height, m_width;

	// Calculates optimal path between a creature and a tile using A* algorithm
	std::vector<QSimulationTile*>path_to_target(const creature* creature, QSimulationTile* target_tile);

	// Calculates optimal path between two tiles using A* algorithm
	std::vector<QSimulationTile*>path_to_target(QSimulationTile* start_tile, QSimulationTile* target_tile);

	// Converts floats to its corresponding TERRAIN_TYPE 
	static TERRAIN_TYPE float_to_terrain_type(float input);

	// Converts TERRAIN_TYPE to its corresponding to its integer representation, used for indexing 
	static int terrain_type_to_int(TERRAIN_TYPE terrain_type);

	// Converts integer representing a terrain type to its enum representation 
	static TERRAIN_TYPE int_to_terrain_type(int terrain_type_idx);

	//Converts 2D array coordinates to linear vector index
	static int coordinate_to_index(int x, int y, int y_dim);

	// Returns the bias for passing a tile by the type of the creature and type of the tile
	static int get_terrain_bias(TERRAIN_TYPE terrain_type, PROPERTIES type);

	world(int x_dim, int y_dim);	

private:

	/* Expands given node to check its adjacent tiles */
	void expand_tile(QSimulationTile* current_tile, QSimulationTile* target_tile, 
						std::vector<QSimulationTile*>* open_list, std::list<QSimulationTile*>* closed_list, PROPERTIES tedrrain_type);

	//Returns vector with pointer to all adjacent tiles of the inputed tile 
	std::vector<QSimulationTile*>get_adjacent_tiles(QSimulationTile* current_tile);

	// Returns the shortes path, by following the predecessors
	void get_path(QSimulationTile* tile, std::vector<QSimulationTile*>* tiles);

	// Sets values back for the next pathfinding
	void clear_predecessors();
};