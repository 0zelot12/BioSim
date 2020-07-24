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

class world
{
public:
	/* Converts floats to its corresponding TERRAIN_TYPE */
	static TERRAIN_TYPE float_to_terrain_type	(float input);					
	static int terrain_type_to_int				(TERRAIN_TYPE terrain_type);	/* Converts TERRAIN_TYPE to its corresponding to its integer represantation, used for indexing */
	static int coordinate_to_index				(int x, int y, int y_dim);		/* Converts 2D array coordinates to linear vector index */

	std::vector<TERRAIN_TYPE>				terrain_map;		/*  */
	std::vector<std::shared_ptr<creature>>	creatures_total;	/* Contains pointers to all creature instances placed on the map */

	world(int x_dim, int y_dim);								/* */

private:
	void add_creature(creature_type type, int x_pos, int y_pos);
};