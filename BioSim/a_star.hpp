#include <string>
#include <vector>
#include <queue>

#include "creature.hpp"
#include "QSimulationTile.hpp"
#include "world.hpp"

struct node
{
	QSimulationTile* tile;
	QSimulationTile* last_tile = nullptr;
	int g_cost, f_cost;
};

class a_star
{
public:
	std::vector<QSimulationTile> calc_path(const std::shared_ptr<creature> creature, QSimulationTile* target_tile);
	a_star(const world& world);

private:
	void expand_node(QSimulationTile* current_node, QSimulationTile* target_tile);
	int calc_distance(POINT_2D start_point, POINT_2D end_point);

	std::vector<QSimulationTile*>							open_list;
	std::vector<QSimulationTile*>							closed_list;
	world													m_world;
};