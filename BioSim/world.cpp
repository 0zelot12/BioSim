#include "world.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: world
//
//
/////////////////////////////////////////////////////////////

struct compare_f_distance
{
    inline bool operator() (tile* tile_1, tile* tile_2)
    {
        return (tile_1->m_f_distance > tile_2->m_f_distance);
    }
};

float manhatten_distance(POINT_2D pos1, POINT_2D pos2)
{
    return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
}

void world::get_path(tile* arg, std::vector<tile*>* tiles)
{
    tile* pre = arg->m_predecessor;

    while (pre)
    {
        tiles->push_back(pre);
        pre = pre->m_predecessor;
    }
}

void world::clear_predecessors()
{
    for (auto& tile : m_tile_map)
    {
        tile.m_predecessor = nullptr;
        tile.m_f_distance = 0;
        tile.m_g_distance = 0;
        tile.m_h_distance = 0;
    }

}

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
    {TERRAIN_TYPE::SNOW,           -1}
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

TERRAIN_TYPE world::int_to_terrain_type(int terrain_type_idx)
{
    return m_int_to_terrain_type.find(terrain_type_idx)->second;
}

int world::coordinate_to_index(int x, int y, int y_dim)
{
    return x + (y * y_dim);
}

int world::get_terrain_bias(TERRAIN_TYPE terrain_type, PROPERTIES type)
{
    if (type == PROPERTIES::LANDBEWOHNER)
    {
        return m_terrain_bias_land.find(terrain_type)->second;
    }
    
    else
    {
        return m_terrain_bias_water.find(terrain_type)->second;
    }
}

bool world::add_creature(const std::shared_ptr<creature_type>& type, int position)
{
    std::shared_ptr<creature> new_creature = std::make_shared<creature>(type->strength(), type->speed(), 
        position, type->name(), type->property_list(), type);

    TERRAIN_TYPE terrain_type = m_tile_map[new_creature->m_current_position].m_terrain_type;

    if (new_creature->terrain_type() == PROPERTIES::LANDBEWOHNER && (terrain_type == TERRAIN_TYPE::DEEP_WATER || terrain_type == TERRAIN_TYPE::SHALLOW_WATER))
    {
        return false;
    }

    else if (new_creature->terrain_type() == PROPERTIES::WASSERBEWOHNER && 
        (terrain_type == TERRAIN_TYPE::SAND || terrain_type == TERRAIN_TYPE::EARTH 
                                            || terrain_type == TERRAIN_TYPE::STONE 
                                            || terrain_type == TERRAIN_TYPE::SNOW))
    {
        return false;
    }

    m_creature_map.push_back(new_creature);
    m_tile_map[position].m_creatures_on_tile.push_back(new_creature);

    return true;
}

std::vector<tile*> world::path_to_target(const std::shared_ptr<creature> creature, tile* target_tile)
{
    return path_to_target(&m_tile_map[creature->m_current_position], target_tile);
}

std::vector<tile*> world::path_to_target(tile* start_tile, tile* target_tile)
{
    std::vector<tile*> path;

    std::unordered_set<tile*> closed_list;

    std::unordered_set<tile*> values_on_open_list;
    std::priority_queue<tile*, std::vector<tile*>, compare_f_distance> open_list;

    open_list.push(start_tile);
    values_on_open_list.insert(start_tile);

    PROPERTIES tp;

    TERRAIN_TYPE start_tile_terrain_type = start_tile->m_terrain_type;
    TERRAIN_TYPE target_tile_terrain_type = target_tile->m_terrain_type;

    if (start_tile_terrain_type == TERRAIN_TYPE::DEEP_WATER || start_tile_terrain_type == TERRAIN_TYPE::SHALLOW_WATER)
    {
        tp = PROPERTIES::WASSERBEWOHNER;
    }

    else
    {
        tp = PROPERTIES::LANDBEWOHNER;
    }

    if (tp == PROPERTIES::LANDBEWOHNER && (target_tile_terrain_type == TERRAIN_TYPE::DEEP_WATER || target_tile_terrain_type == TERRAIN_TYPE::SHALLOW_WATER))
    {
        clear_predecessors();
        return path;
    }

    if (tp != PROPERTIES::LANDBEWOHNER && !(target_tile_terrain_type == TERRAIN_TYPE::DEEP_WATER || target_tile_terrain_type == TERRAIN_TYPE::SHALLOW_WATER))
    {
        clear_predecessors();
        return path;
    }

    while (!open_list.empty())
    {
        if (open_list.size() > 5000)
        {
            clear_predecessors();
            return path;
        }

        tile* current_tile = open_list.top();
        values_on_open_list.erase(current_tile);
        open_list.pop();

        if (current_tile == target_tile)
        {
            get_path(target_tile, &path);

            clear_predecessors();
            return path;
        }

        closed_list.insert(current_tile);

        auto adjacent_tiles = get_adjacent_tiles(current_tile);
        for (auto adjacent_tile : adjacent_tiles)
        {
            // Not all tiles have exactly 8 adjacent tiles
            if (!adjacent_tile)
                continue;

            TERRAIN_TYPE terrain_type = adjacent_tile->m_terrain_type;
            int terrain_bias = get_terrain_bias(terrain_type, tp);

            // Bias of -1 means that the tile isn't walkable
            if (terrain_bias == -1)
            {
                closed_list.insert(adjacent_tile);
                continue;
            }

            // Skip if tile is already on closed_list 
            if (closed_list.find(adjacent_tile) != closed_list.end())
                continue;

            int tentative_g = current_tile->m_g_distance + terrain_bias;

            // Calculate manhatten distance between this tile and the target
            adjacent_tile->m_h_distance = manhatten_distance(adjacent_tile->m_pos, target_tile->m_pos);

            // Skip if tile is already on open list and new distance is equal or longer
            if ((values_on_open_list.find(adjacent_tile) != values_on_open_list.end()) && tentative_g >= adjacent_tile->m_g_distance)
                continue;

             // Update distances
            adjacent_tile->m_predecessor = current_tile;
            adjacent_tile->m_g_distance = tentative_g;
            adjacent_tile->m_f_distance = adjacent_tile->m_g_distance + adjacent_tile->m_h_distance;

            // Insert to open_list if not already there
            if (values_on_open_list.find(adjacent_tile) == values_on_open_list.end())
            {
                open_list.push(adjacent_tile);
                values_on_open_list.insert(adjacent_tile);
            }
        }
    }

    clear_predecessors();

    return path;
}

std::vector<tile*> world::get_adjacent_tiles(tile* current_tile)
{
    std::vector<tile*> adjacent_tiles;

    // A tile can have a maximum of 8 adjacent tiles
    adjacent_tiles.reserve(8);

    int base_idx = current_tile->m_tile_map_idx;

    // Get indicees of adjacent tiles
    int bottom_left     = base_idx - this->m_width - 1,     
        bottom          = base_idx - this->m_width,         
        bottom_right    = base_idx - this->m_width + 1,     
        left            = base_idx - 1,                     
        right           = base_idx + 1,                     
        upper_left      = base_idx + this->m_width - 1,     
        upper           = base_idx + this->m_width,        
        upper_right     = base_idx + this->m_width + 1;   


     // Creatures can't take diagonal paths
     bottom_left  = -1;
     bottom_right = -1;
     upper_left   = -1;
     upper_right  = -1;

    // If a tile is on the edge, it has no adjacent tiles in certain positions
    if (current_tile->m_is_right_edge)
    {
        right          = -1;
        bottom_right   = -1;
        upper_right    = -1;
    }

    if (current_tile->m_is_left_edge)
    {
        left          = -1;
        upper_left    = -1;
        bottom_left   = -1;
    }

        std::vector<int> indicees =
        {                               bottom_left,
                                        bottom,
                                        bottom_right,
                                        left,
                                        right,
                                        upper_left,
                                        upper,
                                        upper_right
        };

 

    // Some tiles doesn't have adjacent tiles in all positions, so set missing to nullptr to skip them later
    for (auto index : indicees)
    {
        if (index > (this->m_tile_map.size() - 1) || index < 0)
        {
            adjacent_tiles.push_back(nullptr);
        }

        else
        {
            adjacent_tiles.push_back(&this->m_tile_map[index]);
        }
    }

    return adjacent_tiles;
}

world::world(int x_dim, int y_dim)
    :
    m_height(y_dim),
    m_width (x_dim)
{
    //Set size of map
    m_tile_map.resize(x_dim * y_dim);

    //Set noise function and its parameter
    FastNoise noise_module;
    noise_module.SetNoiseType(FastNoise::Perlin);
    srand(time(NULL));
    noise_module.SetSeed(rand() % 10000);
    noise_module.SetFrequency(0.03);

    // Fill terrain types to vector
    for (int x = 0; x < x_dim; x++)
    {
        for (int y = 0; y < y_dim; y++)
        {
            int lin_idx = coordinate_to_index(x, y, y_dim);

            // Calculate linear vector 
            m_tile_map[lin_idx].m_terrain_type = float_to_terrain_type(noise_module.GetNoise(x, y));
            m_tile_map[lin_idx].m_tile_map_idx = lin_idx;
            m_tile_map[lin_idx].m_pos.x = x;
            m_tile_map[lin_idx].m_pos.y = y;

            if (lin_idx % y_dim == 0)
                m_tile_map[lin_idx].m_is_left_edge = true;

            if (lin_idx % y_dim == y_dim - 1)
                m_tile_map[lin_idx].m_is_right_edge = true;
        }
    }
}