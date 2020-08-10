#pragma once

#include <map>
#include <vector>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>

#include "image.hpp"
#include "FastNoise.h"
#include "creature.hpp"
#include "QSimulationTile.hpp"

#include <vector>
#include <memory>

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
* @struct POINT_2D
*
* Simple 2D point with x and y coordinate
*************************************************************************/
struct POINT_2D 
{
	int x;
	int y;
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
	/* Converts floats to its corresponding TERRAIN_TYPE */
	static TERRAIN_TYPE float_to_terrain_type		(float input);					
	/* Converts TERRAIN_TYPE to its corresponding to its integer represantation, used for indexing */
	static int			terrain_type_to_int			(TERRAIN_TYPE terrain_type);	
	/* Converts 2D array coordinates to linear vector index */
	static int			coordinate_to_index			(int x, int y, int y_dim);	
	/* Converts linear vector index to 2D array coordinate*/
	static POINT_2D		index_to_coordinate			(int idx, int y_dim);

	/* Contains the type of each tile */
	std::vector<TERRAIN_TYPE>						terrain_map;
	/* Contains all tiles */
	std::vector<QSimulationTile*>					tile_map;
	/* Contains pointers to all creature instances placed on the map */
	std::vector<std::shared_ptr<creature>>			creature_map;	
	/* Returns vector with pointer to all adjacent tiles of the inputed tile */
	std::vector<QSimulationTile*> get_adjacent_tiles(QSimulationTile* current_tile);

	world(int x_dim, int y_dim);	

private:
	/* Height and width of the world in number of tiles */
	unsigned int m_height, m_width;
};