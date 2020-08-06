#include "a_star.hpp"


struct comp_f_distance
{
	inline bool operator() (QSimulationTile* tile1, QSimulationTile* tile2)
	{
		return (tile1->f_distance < tile2->f_distance);
	}
};

std::vector<QSimulationTile> a_star::calc_path(std::shared_ptr<creature> creature, QSimulationTile* target_tile)
{
	std::vector<QSimulationTile> path_tiles;

	this->open_list.push_back(this->m_world.tile_map[creature->m_current_position]);

	QSimulationTile* current_tile = open_list.back();

	while (!open_list.empty())
	{
		if (open_list.size() > 262144)
			return path_tiles;

		current_tile = open_list.back();
		open_list.pop_back();

		if (current_tile == target_tile)
		{
			return path_tiles;
		}

		closed_list.push_back(current_tile);

		this->expand_node(current_tile, target_tile);

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
	for (auto adjacent_tile : adjacent_tiles)
	{
		if (std::find(this->closed_list.begin(), this->closed_list.end(), adjacent_tile) != this->closed_list.end())
			continue;

		Point2D start = world::index_to_coordinate(adjacent_tile->m_tile_map_idx, 128);
		Point2D end = world::index_to_coordinate(target_tile->m_tile_map_idx, 128);

		int h_cost = calc_distance(start, end);
		int g_cost = current_node->g_distance + 1;

		if (std::find(this->open_list.begin(), this->open_list.end(), adjacent_tile) != this->open_list.end())
			continue;

		adjacent_tile->m_last_tile = current_node;
		adjacent_tile->g_distance = g_cost;
		adjacent_tile->h_value = h_cost;

		int f_value = h_cost + g_cost;

		for (auto test : open_list)
		{
			if (test == adjacent_tile)
			{
				test->f_distance = f_value;
				return;
			}
		}

		adjacent_tile->f_distance = f_value;
		open_list.push_back(adjacent_tile);
	}
}

int a_star::calc_distance(Point2D start_point, Point2D end_point)
{
	return std::abs(start_point.x - end_point.x) + std::abs(start_point.y - end_point.y);
}

