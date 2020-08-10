#include "a_star.hpp"


struct comp_f_distance
{
	inline bool operator() (QSimulationTile* tile1, QSimulationTile* tile2)
	{
		return (tile1->f_distance > tile2->f_distance);
	}
};

/////////////////////////////////////////////////////////////
//
//
//  Class: a_star
//
//
/////////////////////////////////////////////////////////////

std::vector<QSimulationTile> a_star::calc_path(std::shared_ptr<creature> creature, QSimulationTile* target_tile)
{
	std::vector<QSimulationTile> path_tiles;

	// Get position of selected creatur
	QSimulationTile* current_tile = this->m_world.tile_map[creature->m_current_position];
	open_list.push_back(current_tile);

	while (!open_list.empty())
	{
		// Avoid out of memory exceptions
		if (open_list.size() > 262144)
			return path_tiles;

		// Get tile with the least f distance and remove it from the list
		current_tile = open_list.back();
		open_list.pop_back();


		if (current_tile == target_tile)
		{
			/* CALCULATE THE PATH BACK */
			return path_tiles;
		}

		/* The node was visited and so we don't have to come back */
		closed_list.push_back(current_tile);

		this->expand_node(current_tile, target_tile);

		/* Sort the list for the next iteration */
		std::sort(open_list.begin(), open_list.end(), comp_f_distance());
	}

	return path_tiles;
}

a_star::a_star(const world& world)
	:
	m_world(world)
{
}

void a_star::expand_node(QSimulationTile* current_node, QSimulationTile* target_tile)
{
	auto adjacent_tiles = m_world.get_adjacent_tiles(current_node);

	for (auto tile : adjacent_tiles)
	{
		/* Skip if tile was already visited to avoid endless loops*/
		if (std::find(this->closed_list.begin(), this->closed_list.end(), tile) != this->closed_list.end())
			continue;

		/* Get the 2D coordinates of the current tile (start) and the target tile (end) */
		POINT_2D start = world::index_to_coordinate(tile->m_tile_map_idx, 128);		
		POINT_2D end =	world::index_to_coordinate(target_tile->m_tile_map_idx, 128);

		int h_cost = calc_distance(start, end);

		int g_cost = current_node->g_distance + 1;

		/* Check if tile is already in open_list */
		if (std::find(this->open_list.begin(), this->open_list.end(), tile) != this->open_list.end() /* UNVOLLSTÄNDIG */)
			continue;

		/* Set parent node and distances */
		tile->m_last_tile = current_node;
		tile->g_distance = g_cost;
		tile->h_value = h_cost;
		int f_value = h_cost + g_cost;

		/* Check if tile is in open_list, and update the tile */
		for (auto open_list_tile : open_list)
		{
			if (open_list_tile == tile)
			{
				open_list_tile->f_distance = f_value;
				return;
			}
		}

		/* Put tile in open_list otherwise */
		tile->f_distance = f_value;
		open_list.push_back(tile);
	}
}

int a_star::calc_distance(POINT_2D start_point, POINT_2D end_point)
{
	return std::abs(start_point.x - end_point.x) + std::abs(start_point.y - end_point.y);
}

