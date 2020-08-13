#include "world.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: path_not_avaiable
//
//
/////////////////////////////////////////////////////////////

struct compare_f_distance
{
    inline bool operator() (QSimulationTile* tile_1, QSimulationTile* tile_2)
    {
        return (tile_1->m_f_distance > tile_2->m_f_distance);
    }
};

float distance(qreal x1, qreal y1, qreal x2, qreal y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

void world::get_predecessor_recursive(QSimulationTile* tile, std::vector<QSimulationTile*>* tiles)
{
    if (tile->m_predecessor) 
    {
        tiles->push_back(tile);
        get_predecessor_recursive(tile->m_predecessor, tiles);

    }

    else
    {
        tiles->push_back(tile);
        tile->m_predecessor = nullptr;
    }

    /* Set predecessor to nullptr for the next pathfinding, otherwise the pointer will still be set */
    clear_predecessors();
    return;
}

void world::get_predecessor_iterative(QSimulationTile* tile, std::vector<QSimulationTile*>* tiles)
{
    auto pre = tile->m_predecessor;
    while (pre)
    {
        tiles->push_back(pre);
        pre = pre->m_predecessor;
    }
    clear_predecessors();
}

void world::clear_predecessors()
{
    for (auto tile : m_tile_map)
    {
        tile->m_predecessor = nullptr;
    }
}

/////////////////////////////////////////////////////////////
//
//
//  Class: world
//
//
/////////////////////////////////////////////////////////////

TERRAIN_TYPE world::float_to_terrain_type(float input)
{
    if (input >= -1 && input <= -0.25)
    {
        return TERRAIN_TYPE::DEEP_WATER;
    }

    else if (input >= -0.25 && input <= 0)
    {
        return TERRAIN_TYPE::SHALLOW_WATER;
    }
    else if (input >= 0 && input <= 0.1)
    {
        return TERRAIN_TYPE::SAND;
    }

    else if (input >= 0.1 && input <= 0.125)
    {
        return TERRAIN_TYPE::EARTH;
    }

    else if (input >= 0.125 && input <= 0.3)
    {
        return TERRAIN_TYPE::STONE;
    }

    else if (input >= 0.3 && input <= 1)
    {
        return TERRAIN_TYPE::SNOW;
    }

    else {
        return TERRAIN_TYPE::DEEP_WATER;
    }
}

const std::map<TERRAIN_TYPE, int> m_terrain_type_to_int =
{
    {TERRAIN_TYPE::DEEP_WATER,      0},
    {TERRAIN_TYPE::EARTH,           1},
    {TERRAIN_TYPE::STONE,           2},
    {TERRAIN_TYPE::SAND,            3},
    {TERRAIN_TYPE::SHALLOW_WATER,   4},
    {TERRAIN_TYPE::SNOW,            5}
};

const std::map<TERRAIN_TYPE, int> m_terrain_bias_land =
{
    {TERRAIN_TYPE::DEEP_WATER,     -1},
    {TERRAIN_TYPE::EARTH,           1},
    {TERRAIN_TYPE::STONE,           4},
    {TERRAIN_TYPE::SAND,            1},
    {TERRAIN_TYPE::SHALLOW_WATER,  -1},
    {TERRAIN_TYPE::SNOW,            2}
};

const std::map<TERRAIN_TYPE, int> m_terrain_bias_water =
{
    {TERRAIN_TYPE::DEEP_WATER,      3},
    {TERRAIN_TYPE::EARTH,          -1},
    {TERRAIN_TYPE::STONE,          -1},
    {TERRAIN_TYPE::SAND,           -1},
    {TERRAIN_TYPE::SHALLOW_WATER,   1},
    {TERRAIN_TYPE::SNOW,            5}
};

const std::map<int, TERRAIN_TYPE> m_int_to_terrain_type =
{
    {0, TERRAIN_TYPE::DEEP_WATER    },
    {1, TERRAIN_TYPE::EARTH         },
    {2, TERRAIN_TYPE::STONE         },
    {3, TERRAIN_TYPE::SAND          },
    {4, TERRAIN_TYPE::SHALLOW_WATER },
    {5, TERRAIN_TYPE::SNOW          }
};

int world::terrain_type_to_int(TERRAIN_TYPE terrain_type)
{
    return m_terrain_type_to_int.find(terrain_type)->second;
}

TERRAIN_TYPE world::int_to_terrain_type(int terrain_type_idx)
{
    return m_int_to_terrain_type.find(terrain_type_idx)->second;
}

int world::coordinate_to_index(int x, int y, int y_dim)
{
    return x + (y * y_dim);
}

int world::get_terrain_bias(TERRAIN_TYPE terrain_type, bool is_land)
{
    if (is_land)
    {
        return m_terrain_bias_land.find(terrain_type)->second;
    }
    
    else
    {
        return m_terrain_bias_water.find(terrain_type)->second;
    }
}

//std::vector<QSimulationTile*> world::path_to_target(const creature* creature, QSimulationTile* target_tile)
//{
//    std::vector <QSimulationTile*> path;
//    std::vector <QSimulationTile*> open_list;
//    std::list   <QSimulationTile*> closed_list;
//
//    auto start_tile = this->m_tile_map[creature->m_current_position];
//
//    open_list.push_back(start_tile);
//
//    while (!open_list.empty())
//    {
//        QSimulationTile* current_tile = open_list.back();
//        open_list.pop_back();
//
//        if (current_tile == target_tile)
//        {
//            get_predecessor(target_tile, &path);
//            return path;
//        }
//
//        closed_list.push_front(current_tile);
//
//        expand_node(current_tile, target_tile, &open_list, &closed_list);
//
//        /* Sort the list by f_values for the next iteration */
//        std::sort(open_list.begin(), open_list.end(), compare_f_distance());
//    }
//
//    return path;
//
//}

std::vector<QSimulationTile*> world::path_to_target(QSimulationTile* start_tile, QSimulationTile* target_tile)
{
    std::vector <QSimulationTile*> path;
    std::vector <QSimulationTile*> open_list;
    std::list   <QSimulationTile*> closed_list;

    open_list.push_back(start_tile);

    bool is_land = true;

    TERRAIN_TYPE start_tile_terrain_type    = int_to_terrain_type(start_tile->m_terrain_type_idx    );
    TERRAIN_TYPE target_tile_terrain_type   = int_to_terrain_type(target_tile->m_terrain_type_idx   );

    if (start_tile_terrain_type == TERRAIN_TYPE::DEEP_WATER || start_tile_terrain_type == TERRAIN_TYPE::SHALLOW_WATER)
        is_land = false;

    if (is_land && (target_tile_terrain_type == TERRAIN_TYPE::DEEP_WATER || target_tile_terrain_type == TERRAIN_TYPE::SHALLOW_WATER))
    {
        return path;
    }

    if (!is_land && !(target_tile_terrain_type == TERRAIN_TYPE::DEEP_WATER || target_tile_terrain_type == TERRAIN_TYPE::SHALLOW_WATER))
    {
        return path;
    }

    while (!open_list.empty())
    {
        if (open_list.size() > 5000)
        {
            for (auto tile : closed_list)
            {
                tile->m_predecessor = nullptr;
            }

            for (auto tile : open_list)
            {
                tile->m_predecessor = nullptr;
            }

            return path;
        }

        QSimulationTile* current_tile = open_list.back();
        open_list.pop_back();

        if (current_tile == target_tile)
        {
            get_predecessor_recursive(target_tile, &path);
            return path;
        }

        closed_list.push_front(current_tile);

        expand_tile(current_tile, target_tile, &open_list, &closed_list, is_land);

        /* Sort the list by f_values for the next iteration */
        std::sort(open_list.begin(), open_list.end(), compare_f_distance());
    }

    for (auto tile : closed_list)
    {
        tile->m_predecessor = nullptr;
    }

    return path;
}

std::vector<QSimulationTile*> world::get_adjacent_tiles(QSimulationTile* current_tile)
{
    std::vector<QSimulationTile*> adjacent_tiles;
    adjacent_tiles.reserve(8);

    int base_idx = current_tile->m_tile_map_idx;

    int north_west  = base_idx - this->m_width - 1,
        north       = base_idx - this->m_width,
        north_east  = base_idx - this->m_width + 1,
        west        = base_idx - 1,
        east        = base_idx + 1,
        south_west  = base_idx + this->m_width - 1,
        south       = base_idx + this->m_width,
        south_east  = base_idx + this->m_width + 1;

    std::vector<int> indicees =
    {                               north_west,
                                    north,north_east,
                                    west,
                                    east,
                                    south_west,
                                    south,
                                    south_east 
    };

    /* Some tiles doesn't have adjacent tiles in all positions, 
    so set missing to nullptr to skip them later */
    for (auto index : indicees)
    {
        if (index > (this->m_tile_map.size() - 1) || index < 0)
        {
            adjacent_tiles.push_back(nullptr);
        }

        else
        {
            adjacent_tiles.push_back(this->m_tile_map.at(index));
        }
    }

    return adjacent_tiles;
}

world::world(int x_dim, int y_dim)
    :
    m_height(y_dim),
    m_width (x_dim)
{
    /* Set size of map */
    m_terrain_map.resize(x_dim * y_dim);

    /* Set noise function and its parameter */
    FastNoise noise_module;
    noise_module.SetNoiseType(FastNoise::Perlin);

    srand(time(NULL));
    noise_module.SetSeed        (rand() % 10000);
    noise_module.SetFrequency   (0.03);

    /* Fill terrain types to vector */
    for (int x = 0; x < x_dim; x++)
    {
        for (int y = 0; y < y_dim; y++)
        {
            /* Calculate linear vector */
            m_terrain_map[coordinate_to_index(x, y, y_dim)] = float_to_terrain_type(noise_module.GetNoise(x, y));
        }
    }
}

void world::expand_tile(QSimulationTile* current_tile, QSimulationTile* target_tile, std::vector<QSimulationTile*>* open_list, std::list<QSimulationTile*>* closed_list, bool is_land)
{
    auto adjacent_tiles = get_adjacent_tiles(current_tile);

    for (auto adjacent_tile : adjacent_tiles)
    {
        if (!adjacent_tile)
            continue;

        TERRAIN_TYPE    terrain_type = int_to_terrain_type  (adjacent_tile->m_terrain_type_idx);
        int             terrain_bias = get_terrain_bias     (terrain_type, is_land);

        if (terrain_bias == -1)
        {
            closed_list->push_back(adjacent_tile);
            continue;
        }


        /* Skip if tile is already on closed_list */
        if (std::find(closed_list->begin(), closed_list->end(), adjacent_tile) != closed_list->end())
            continue;
        
        int tentative_g = current_tile->m_g_distance + terrain_bias;
        adjacent_tile->m_h_distance = distance(adjacent_tile->pos().x(), adjacent_tile->pos().y(), target_tile->pos().x(),target_tile->pos().y());

        /* Skip if tile is already on open list and new distance is equal or longer */
        if ((std::find(open_list->begin(), open_list->end(), adjacent_tile) != open_list->end()) && tentative_g >= adjacent_tile->m_g_distance)
            continue;

        /* Update distances */
        adjacent_tile->m_predecessor = current_tile;
        adjacent_tile->m_g_distance = tentative_g;
        adjacent_tile->m_f_distance = adjacent_tile->m_g_distance + adjacent_tile->m_h_distance;

        /* Insert to open_list if not already there, otherwise values are updated since where using pointers */
        if (std::find(open_list->begin(), open_list->end(), adjacent_tile) == open_list->end())
        {
            open_list->push_back(adjacent_tile);
        }
    }

    return;
}