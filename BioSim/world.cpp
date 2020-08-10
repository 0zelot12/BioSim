#include "world.hpp"

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

int world::terrain_type_to_int(TERRAIN_TYPE terrain_type)
{
    return m_terrain_type_to_int.find(terrain_type)->second;
}

int world::coordinate_to_index(int x, int y, int y_dim)
{
    return x + (y * y_dim);
}

POINT_2D world::index_to_coordinate(int idx, int y_dim)
{
    POINT_2D point;

    point.y = idx / y_dim;
    point.x = idx - (point.y * y_dim);

    return point;
}

std::vector<QSimulationTile*> world::get_adjacent_tiles(QSimulationTile* current_tile)
{
    std::vector<QSimulationTile*> adjacent_tiles;
    adjacent_tiles.reserve(8);

    POINT_2D point = index_to_coordinate(current_tile->m_tile_map_idx, this->m_height);

    adjacent_tiles.push_back(this->tile_map.at(coordinate_to_index(point.x - 1,     point.y - 1,    this->m_width)));
    adjacent_tiles.push_back(this->tile_map.at(coordinate_to_index(point.x,         point.y - 1,    this->m_width)));
    adjacent_tiles.push_back(this->tile_map.at(coordinate_to_index(point.x + 1,     point.y - 1,    this->m_width)));

    adjacent_tiles.push_back(this->tile_map.at(coordinate_to_index(point.x - 1,     point.y,        this->m_width)));
    adjacent_tiles.push_back(this->tile_map.at(coordinate_to_index(point.x + 1,     point.y,        this->m_width)));

    adjacent_tiles.push_back(this->tile_map.at(coordinate_to_index(point.x - 1,     point.y + 1,    this->m_width)));
    adjacent_tiles.push_back(this->tile_map.at(coordinate_to_index(point.x,         point.y + 1,    this->m_width)));
    adjacent_tiles.push_back(this->tile_map.at(coordinate_to_index(point.x + 1,     point.y + 1,    this->m_width)));

    return adjacent_tiles;
}

world::world(int x_dim, int y_dim)
    :
    m_height(y_dim),
    m_width(x_dim)
{
    /* Set size of map */
    terrain_map.resize(x_dim * y_dim);

    /* Set noise function and its parameter */
    FastNoise noise_module;
    noise_module.SetNoiseType(FastNoise::Perlin);

    srand(time(NULL));
    noise_module.SetSeed(rand() % 10000);
    noise_module.SetFrequency(0.03);

    /* Fill terrain types to vector */
    for (int x = 0; x < x_dim; x++)
    {
        for (int y = 0; y < y_dim; y++)
        {
            /* Calculate linear vector */
            terrain_map[coordinate_to_index(x, y, y_dim)] = float_to_terrain_type(noise_module.GetNoise(x, y));
        }
    }
}