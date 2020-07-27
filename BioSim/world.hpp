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

enum class TERRAIN_TYPE {	DEEP_WATER, 
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
	/* Converts floats to its corresponding TERRAIN_TYPE */
	static TERRAIN_TYPE float_to_terrain_type	(float input);					
	/* Converts TERRAIN_TYPE to its corresponding to its integer represantation, used for indexing */
	static int terrain_type_to_int				(TERRAIN_TYPE terrain_type);	
	/* Converts 2D array coordinates to linear vector index */
	static int coordinate_to_index				(int x, int y, int y_dim);		
	
	/* Contains the type of each tile */
	std::vector<TERRAIN_TYPE>				terrain_map;
	/* Contains pointers to all creature instances placed on the map */
	std::vector<creature*>					creatures_total;	

	world(int x_dim, int y_dim);							
};