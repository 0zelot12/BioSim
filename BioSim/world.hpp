#pragma once

#include <map>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include <queue>

#include "FastNoise.h"

#include "image.hpp"
#include "creature.hpp"
#include "tile.hpp"

/**
*************************************************************************
* @class world
*
* Class holding the content of the simulation
*************************************************************************/
class world
{
public:

	// Constructor takes dimensions of the map in tiles
	world(int x_dim, int y_dim);

	// Contains all tiles 
	std::vector<tile> m_tile_map;

	// Calculates optimal path between a creature and a tile using A* algorithm
	std::vector<tile*> path_to_target(const std::shared_ptr<creature> creature, tile* target_tile);

	// Calculates optimal path between two tiles using A* algorithm
	std::vector<tile*> path_to_target(tile* start_tile, tile* target_tile);

	// Converts integer representing a terrain type to its enum representation 
	static TERRAIN_TYPE int_to_terrain_type(int terrain_type_idx);

	// Converts 2D array coordinates to linear vector index
	static int coordinate_to_index(int x, int y, int y_dim);

	// Adds creature to the simulation, returns true in case of success
	bool add_creature(const std::shared_ptr<creature_type>& type, int position);

	// Adds creature to the simulation, returns true in case of success
	bool add_creature(const std::shared_ptr<creature_type>& type, int position, double health);

	// Iterates over all creatures and updates their state in simulation
	void simulation_step();

	// Updates the state of the creature according to the rest of the simulation
	void make_transition(std::shared_ptr<creature>& entity);

	//Returns vector with pointer to all adjacent tiles of the inputed tile 
	std::vector<tile*> get_adjacent_tiles(tile* current_tile, int offset);
	std::vector<tile*> get_adjacent_tiles_new(tile* current_tile);

	std::vector<tile*> get_tiles_in_range(tile* current_tile, int offset);

	// Converts vector of tiles to vector of corresponding indicees 
	std::vector<int> get_path_indicees(const std::vector<tile*>& path_tiles);

	// Calculates optimal path between two tiles using A* algorithm
	std::vector<int> path_to_target(const std::shared_ptr<creature> creature_1, const std::shared_ptr<creature> creature_2);

	int simulation_steps_total = 0;

private:

	//Returns vector with pointer to all adjacent tiles of the inputed tile 
	std::vector<tile*> get_adjacent_tiles(tile* current_tile);

	bool is_in_range(int tile_map_idx);
	int mod_idx(int tile_idx);
	bool is_suitable_tile(std::shared_ptr<creature> type, const tile& tile);

	// Returns the shortes path, by following the predecessors
	void get_path(tile* arg, std::vector<tile*>* tiles);

	// Sets values back for the next pathfinding
	void clear_predecessors();

	// Returns the bias for passing a tile by the type of the creature and type of the tile
	static int get_terrain_bias(TERRAIN_TYPE terrain_type, PROPERTIES type);

	// Converts floats to its corresponding TERRAIN_TYPE 
	static TERRAIN_TYPE float_to_terrain_type(float input);

	//Height and width of the world in number of tiles 
	unsigned int m_height, m_width;

	// Contains pointers to all creature instances placed on the map 
	std::vector<std::shared_ptr<creature>> m_creature_map;

	// Returns tile at the given index
	tile* idx_to_tile(int idx);

	// Returns number of creatures of same type in specified range
	int number_of_equal_creatures(const std::shared_ptr<creature>& entity, unsigned int range);

	// Returns number of creatures of same type on specified tile
	int number_of_equal_creatures_on_tile(const std::shared_ptr<creature>& entity, const tile& tile);

	// Returns number of creatures of same type on specified tile
	int number_of_animals_on_tile(const tile& tile);

	// Returns number of creatures of same type on specified tile
	int number_of_plants_on_tile(const tile& tile);

	void plant_wait(std::shared_ptr<creature>& plant, const TERRAIN_TYPE& terrain_type);

	//Returns true in case of success, false otherwise
	bool plant_grow(std::shared_ptr<creature>& plant);

	void animal_rest(std::shared_ptr<creature>& animal);
	void animal_mate(std::shared_ptr<creature>& animal);

	// Checks reqirements of MATE state, returns true if transition was successful, false otherwise
	bool mate_transition(std::shared_ptr<creature>& animal);

	// Checks reqirements of HUNT state, returns true if transition was successful, false otherwise
	bool hunt_transition(std::shared_ptr<creature>& animal);
	void animal_hunt(std::shared_ptr<creature>& animal);

	// Sets random target for wandering
	void animal_set_wander_target(std::shared_ptr<creature>& animal);
	// Make single wanderstep
	void animal_make_wander_step(std::shared_ptr<creature>& animal);

	void animal_attack(std::shared_ptr<creature>& animal);

	// Sets state of entity to DEAD
	void entity_die(std::shared_ptr<creature>& entity);

};