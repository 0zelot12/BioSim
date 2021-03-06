#include "world.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: world
//
//
/////////////////////////////////////////////////////////////

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
    tiles->push_back(arg);
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

void world::make_transition(std::shared_ptr<creature>& entity)
{ 
    if (entity->entity_type() == PROPERTIES::PFLANZE)
    {

        if (entity->m_current_state == STATE::NEW)
        {
            entity->m_current_state = STATE::WAIT;
        }

        if (entity->m_steps_since_last_rest > entity->m_health / 100)
        {
            int count = number_of_equal_creatures(entity, 5);

            if (count > 2 && count < 10)
            {
                if (plant_grow(entity))
                {
                    entity->m_steps_since_last_rest = 0;
                }

                else
                {
                    plant_wait(entity, m_tile_map[entity->m_current_position].m_terrain_type);
                }
            }

            else
            {
                plant_wait(entity, m_tile_map[entity->m_current_position].m_terrain_type);
            }
        }

        else
        {
            plant_wait(entity, m_tile_map[entity->m_current_position].m_terrain_type);
        }
    }

    if (entity->entity_type() == PROPERTIES::TIER)
    {
        if (entity->m_current_state == STATE::NEW)
        {
            entity->m_current_state = STATE::WANDER;
        }

        if (entity->m_current_state == STATE::WANDER)
        {
            if (mate_transition(entity)) 
            {
                return;
            }

            else
            {
                entity->m_steps_since_last_replication++;
            }

            if (hunt_transition(entity))
            {
                return;
            }

            if (entity->m_path_to_current_target.empty())
            {
                animal_set_wander_target(entity);
            }

            animal_make_wander_step(entity);
        }

        else if (entity->m_current_state == STATE::MATE)
        {
            animal_mate(entity);
            entity->m_steps_since_last_replication = 0;
            entity->m_current_state = STATE::WANDER;
        }

        else if (entity->m_current_state == STATE::REST)
        {
            animal_rest(entity);
        }

        else if (entity->m_current_state == STATE::HUNT)
        {
            animal_hunt(entity);
        }

        else if (entity->m_current_state == STATE::ATTACK)
        {
            animal_attack(entity);
        }
    }
}

tile* world::idx_to_tile(int idx)
{
    if (idx > 0 && m_tile_map.size() > idx)
    {
        return &m_tile_map[idx];
    }

    return nullptr;
}

int world::number_of_equal_creatures(const std::shared_ptr<creature>& entity, unsigned int distance)
{
    int count = 0;
    auto adjacent_tiles = world::get_tiles_in_range(&m_tile_map[entity->m_current_position], distance);

    for (auto current_tile : adjacent_tiles)
    {
        count += number_of_equal_creatures_on_tile(entity, *current_tile);
    }

    return count;
}

int world::number_of_equal_creatures_on_tile(const std::shared_ptr<creature>& entity, const tile& tile)
{
    int count = 0;

    for (auto& entity_on_tile : tile.m_creatures_on_tile)
    {
        if (creature::have_same_type(entity, entity_on_tile))
        {
            count++;
        }
    }

    return count;
}

void world::entity_die(std::shared_ptr<creature>& entity)
{
    entity->m_current_state = STATE::DEAD;
}

int world::number_of_animals_on_tile(const tile& tile)
{
    int count = 0;

    for (auto& entity_on_tile : tile.m_creatures_on_tile)
    {
        if (entity_on_tile->m_types.entity_type == PROPERTIES::TIER)
        {
            count++;
        }
    }

    return count;
}

int world::number_of_plants_on_tile(const tile& tile)
{
    int count = 0;

    for (auto& entity_on_tile : tile.m_creatures_on_tile)
    {
        if (entity_on_tile->m_types.entity_type == PROPERTIES::PFLANZE)
        {
            count++;
        }
    }

    return count;
}

void world::plant_wait(std::shared_ptr<creature>& plant, const TERRAIN_TYPE& terrain_type)
{
    if (terrain_type == TERRAIN_TYPE::DEEP_WATER || terrain_type == TERRAIN_TYPE::SNOW || terrain_type == TERRAIN_TYPE::STONE)
    {
        plant->m_health -= 25;
    }

    else
    {
        plant->m_health -= 10;
    }

    plant->m_steps_since_last_rest++;
}

bool world::plant_grow(std::shared_ptr<creature>& plant)
{
    auto tiles_in_range = get_tiles_in_range(&m_tile_map[plant->m_current_position], 5);
    std::vector<tile*> suitable_tiles;
    for (auto& tile : tiles_in_range)
    {
        if (is_suitable_tile(plant, *tile) && number_of_equal_creatures_on_tile(plant, *tile) == 0 && number_of_plants_on_tile(*tile) == 0)

            suitable_tiles.push_back(tile);
    }

    if (suitable_tiles.size() > 0)
    {
        for (int i = 0; i < 2; i++)
        {
            // Obtain a random number from hardware
            std::random_device rd; 
            // Seed the generator
            std::mt19937 gen(rd()); 
            // Define the range
            std::uniform_int_distribution<> distr(0, suitable_tiles.size() - 1); 

            add_creature(plant->m_type, suitable_tiles[distr(gen)]->m_tile_map_idx);
        }

        return true;
    }

    return false;
}

void world::animal_set_wander_target(std::shared_ptr<creature>& animal)
{
    auto tiles_in_range = get_tiles_in_range(&m_tile_map[animal->m_current_position], 10);

    // Obtain a random number from hardware
    std::random_device rd;
    // Seed the generator
    std::mt19937 gen(rd());
    // Define the range
    std::uniform_int_distribution<> distr(0, tiles_in_range.size() - 1);

    auto path_tiles = path_to_target(&m_tile_map[animal->m_current_position], tiles_in_range[distr(gen)]);

    if (path_tiles.size() == 0)
        return;

    // Remove tile where creature currently sits, because not needed
    path_tiles.pop_back();

    animal->m_path_to_current_target = get_path_indicees(path_tiles);
}

void world::animal_rest(std::shared_ptr<creature>& animal)
{
    double waiting_threshold = ((double)animal->m_health / (double)animal->m_type->max_health()) * 5;

    // Resting completed, go back to WANDER
    if (animal->m_steps_since_last_rest >= waiting_threshold)
    {
        animal->m_steps_since_last_rest = 0;
        animal->m_current_state = STATE::WANDER;
        return;
    }

    animal->m_health -= 5;
    animal->m_steps_since_last_rest++;
}

void world::animal_mate(std::shared_ptr<creature>& animal)
{
    auto tiles_in_range = get_tiles_in_range(&m_tile_map[animal->m_current_position], 3);
    std::vector<tile*> suitable_tiles;

    for (auto tile : tiles_in_range)
    {
        if (number_of_animals_on_tile(*tile) == 0)
        {
            suitable_tiles.push_back(tile);
        }
    }

    for (int i = 0; i < 2; i++)
    {
        // Obtain a random number from hardware
        std::random_device rd;
        // Seed the generator
        std::mt19937 gen(rd());
        // Define the range
        std::uniform_int_distribution<> distr(0, suitable_tiles.size() - 1);

        add_creature(animal->m_type, suitable_tiles[distr(gen)]->m_tile_map_idx, 0.5 * animal->m_health);
        animal->m_health = animal->m_health * 0.75;
    }
}

bool world::mate_transition(std::shared_ptr<creature>& animal)
{
    if (animal->m_health > (0.5 * animal->m_type->max_health()) && animal->m_steps_since_last_replication > (animal->m_health / 50))
    {
        auto creatures_in_rng = number_of_equal_creatures(animal, 3);

        if (creatures_in_rng > 1 && creatures_in_rng < 5)
        {
            animal->m_current_state = STATE::MATE;
            return true;
        }
    }

    return false;
}

bool world::hunt_transition(std::shared_ptr<creature>& animal)
{
    auto tiles_in_range = get_tiles_in_range(&m_tile_map[animal->m_current_position], 10);

    if (animal->m_health < (0.6 * animal->m_type->max_health()))
    {
        // Search for targets
        for (auto& tile : tiles_in_range)
        {
            if (!tile->m_creatures_on_tile.empty())
            {
                for (auto& prey : tile->m_creatures_on_tile)
                {
                    if (animal->m_types.prey_type == PROPERTIES::FLEISCHFRESSER && 
                        prey->m_types.entity_type == PROPERTIES::TIER && prey->m_current_state != STATE::DEAD && 
                        animal->terrain_type() == tile->m_creatures_on_tile[0]->terrain_type())
                    {
                        animal->m_currrent_target = tile->m_creatures_on_tile[0];
                        animal->m_current_state = STATE::HUNT;
                        return true;
                    }

                    else if (animal->m_types.prey_type == PROPERTIES::PFLANZENFRESSER && 
                        prey->m_types.entity_type == PROPERTIES::PFLANZE && prey->m_current_state != STATE::DEAD && 
                        animal->terrain_type() == tile->m_creatures_on_tile[0]->terrain_type())
                    {
                        animal->m_currrent_target = tile->m_creatures_on_tile[0];
                        animal->m_current_state = STATE::HUNT;
                        return true;
                    }
                }

            }
        }
    }

    return false;
}

void world::animal_hunt(std::shared_ptr<creature>& animal)
{
    animal->m_path_to_current_target = path_to_target(animal, animal->m_currrent_target);

    // Remove tile where creature currently sits, because not needed
    animal->m_path_to_current_target.pop_back();

    if (animal->m_path_to_current_target.size() == 1)
    {
        animal->m_current_state = STATE::ATTACK;
        return;
    }

    double range = ((double)animal->m_speed * 1.0) / 100;

    int terrain_bias;

    if (animal->terrain_type() == PROPERTIES::LANDBEWOHNER)
    {
        terrain_bias = m_terrain_bias_land.find(m_tile_map[animal->m_current_position].m_terrain_type)->second;
    }

    else
    {
        terrain_bias = m_terrain_bias_water.find(m_tile_map[animal->m_current_position].m_terrain_type)->second;
    }

    double effective_range = range / terrain_bias;

    // No path available
    if (animal->m_path_to_current_target.size() <= 0)
        return;

    // Next tile is in y direction
    if (mod_idx(animal->m_current_position) == mod_idx(animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]))
    {

        if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.y >= animal->m_position_y)
        {
            animal->m_position_y += effective_range;

            if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.y <= animal->m_position_y)
            {
                m_tile_map[animal->m_current_position].delete_creature_from_tile(animal);
                animal->m_current_position = animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1];
                m_tile_map[animal->m_current_position].add_creature_to_tile(animal);
                animal->m_health -= 2;
                animal->m_path_to_current_target.pop_back();
            }
        }

        else
        {
            animal->m_position_y -= effective_range;

            if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.y >= animal->m_position_x)
            {
                m_tile_map[animal->m_current_position].delete_creature_from_tile(animal);
                animal->m_current_position = animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1];
                m_tile_map[animal->m_current_position].add_creature_to_tile(animal);
                animal->m_health -= 2;
                animal->m_path_to_current_target.pop_back();
            }
        }
    }

    else
    {
        if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.x >= animal->m_position_x)
        {
            animal->m_position_x += effective_range;

            if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.x <= animal->m_position_x)
            {
                m_tile_map[animal->m_current_position].delete_creature_from_tile(animal);
                animal->m_current_position = animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1];
                m_tile_map[animal->m_current_position].add_creature_to_tile(animal);
                animal->m_position_x = m_tile_map[animal->m_current_position].m_pos.x;
                animal->m_position_y = m_tile_map[animal->m_current_position].m_pos.y;
                animal->m_health -= 2;
                animal->m_path_to_current_target.pop_back();
            }
        }

        else
        {
            animal->m_position_x -= effective_range;

            if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.x >= animal->m_position_x)
            {
                m_tile_map[animal->m_current_position].delete_creature_from_tile(animal);
                animal->m_current_position = animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1];
                m_tile_map[animal->m_current_position].add_creature_to_tile(animal);
                animal->m_health -= 2;
                animal->m_path_to_current_target.pop_back();
            }
        }
    }
}

void world::animal_attack(std::shared_ptr<creature>& animal)
{
    if (animal->m_health == animal->m_type->max_health())
    {
        animal->m_current_state = STATE::REST;
        return;
    }

    if (animal->m_currrent_target->m_health <= 0)
    {
        animal->m_current_state = STATE::REST;
        animal->m_currrent_target->m_current_state = STATE::DEAD;
        animal->m_currrent_target = NULL;
        return;
    }

    animal->m_currrent_target->m_health -= animal->m_strength;
    animal->m_health += animal->m_strength / 2;

    if (path_to_target(animal, animal->m_currrent_target).size() > 0)
    {
        animal->m_current_state = STATE::HUNT;
    }

    else
    {

        return;
    }
}

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
    std::shared_ptr<creature> new_creature = std::make_shared<creature>(type->strength(), type->speed(), type->max_health(), 
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

    new_creature->m_position_x = m_tile_map[position].m_pos.x;
    new_creature->m_position_y = m_tile_map[position].m_pos.y;

    m_creature_map.push_back(new_creature);
    m_tile_map[position].m_creatures_on_tile.push_back(new_creature);

    return true;
}

bool world::add_creature(const std::shared_ptr<creature_type>& type, int position, double health)
{
    std::shared_ptr<creature> new_creature = std::make_shared<creature>(type->strength(), type->speed(), health,
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

    new_creature->m_position_x = m_tile_map[position].m_pos.x;
    new_creature->m_position_y = m_tile_map[position].m_pos.y;

    m_creature_map.push_back(new_creature);
    m_tile_map[position].m_creatures_on_tile.push_back(new_creature);

    return true;
}

void world::simulation_step()
{
    // Shallow copy needed, since we modify m_creature_map
    auto creature_list = m_creature_map; 

    for (auto& entity : creature_list)
    {
        if (entity->m_health <= 0)
        {
            entity_die(entity);
            entity->m_steps_since_death++;

            if (entity->m_steps_since_death >= 10)
            {
                m_tile_map[entity->m_current_position].delete_creature_from_tile(entity);
            }

            continue;
        }

        if (entity->m_current_state != STATE::DEAD)
        {
            make_transition(entity);
        }
    }

    simulation_steps_total++;
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
    std::priority_queue<tile*, std::deque<tile*>, compare_f_distance> open_list;

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
        if (open_list.size() > this->m_height * this->m_width)
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

bool world::is_in_range(int tile_map_idx)
{
    return tile_map_idx >= 0 && (m_tile_map.size() - 1 >= tile_map_idx);
}

int world::mod_idx(int tile_idx)
{
    return tile_idx % this->m_height;
}

bool world::is_suitable_tile(std::shared_ptr<creature> type, const tile& tile)
{
    switch (type->entity_type())
    {
    case PROPERTIES::LANDBEWOHNER:
        if (tile.m_terrain_type == TERRAIN_TYPE::DEEP_WATER || tile.m_terrain_type == TERRAIN_TYPE::SHALLOW_WATER)
            return false;
        break;
    case PROPERTIES::WASSERBEWOHNER:
        if (tile.m_terrain_type == TERRAIN_TYPE::EARTH || tile.m_terrain_type == TERRAIN_TYPE::SAND || tile.m_terrain_type == TERRAIN_TYPE::SNOW || tile.m_terrain_type == TERRAIN_TYPE::STONE)
            return false;
        break;
    default:
        break;
    }

    return true;
}

std::vector<tile*> world::get_adjacent_tiles(tile* current_tile, int offset)
{
    std::vector<tile*> adjacent_tiles;

    int base_idx = current_tile->m_tile_map_idx;
    for (int i = 0; i < offset; i++)
    {
        // Get indicees of adjacent tiles
        int bottom = base_idx - this->m_width - (i * this->m_width),
            left = base_idx - 1 - i,
            right = base_idx + 1 + i,
            upper = base_idx + this->m_width + (i * this->m_width);

        if (left >= 0 && !current_tile->m_is_left_edge && left <= m_tile_map.size() - 1)
            adjacent_tiles.push_back(&m_tile_map[left]);

        if (right >= 0 && !current_tile->m_is_right_edge && right <= m_tile_map.size() - 1)
            adjacent_tiles.push_back(&m_tile_map[right]);

        if (upper >= 0 && upper <= m_tile_map.size() - 1)
            adjacent_tiles.push_back(&m_tile_map[upper]);

        if (bottom >= 0 && bottom <= m_tile_map.size() - 1)
            adjacent_tiles.push_back(&m_tile_map[bottom]);
    }

    return adjacent_tiles;
}

std::vector<tile*> world::get_adjacent_tiles_new(tile* current_tile)
{
    std::vector<tile*> adjacent_tiles;

    // A tile can have a maximum of 8 adjacent tiles
    adjacent_tiles.reserve(8);

    int base_idx = current_tile->m_tile_map_idx;

    // Get indicees of adjacent tiles
    int bottom_left = base_idx - this->m_width - 1,
        bottom = base_idx - this->m_width,
        bottom_right = base_idx - this->m_width + 1,
        left = base_idx - 1,
        right = base_idx + 1,
        upper_left = base_idx + this->m_width - 1,
        upper = base_idx + this->m_width,
        upper_right = base_idx + this->m_width + 1;


    // Creatures can't take diagonal paths
    bottom_left = -1;
    bottom_right = -1;
    upper_left = -1;
    upper_right = -1;

    // If a tile is on the edge, it has no adjacent tiles in certain positions
    if (current_tile->m_is_right_edge)
    {
        right = -1;
        bottom_right = -1;
        upper_right = -1;
    }

    if (current_tile->m_is_left_edge)
    {
        left = -1;
        upper_left = -1;
        bottom_left = -1;
    }

    std::vector<int> indicees =
    { bottom_left,
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
            //adjacent_tiles.push_back(nullptr);
        }

        else
        {
            adjacent_tiles.push_back(&this->m_tile_map[index]);
        }
    }

    return adjacent_tiles;
}

std::vector<tile*> world::get_tiles_in_range(tile* current_tile, int offset)
{
    std::vector<tile*> tiles_in_range;

    int base_idx = current_tile->m_tile_map_idx;

    for (int i = 0; i <= offset; i++)
    {
        for (int j = offset - i; j >= 0; j--)
        {
            // Leave out the start tile
            if (i == 0 && j == 0)
                continue;

            int a = base_idx + i - (j * this->m_width);

            int b = base_idx + i + (j * this->m_width);

            int c = base_idx - i - (j * this->m_width);

            int d = base_idx - i + (j * this->m_width);
            
            if (is_in_range(a) && mod_idx(a) >= mod_idx(base_idx))
                tiles_in_range.push_back(&m_tile_map[a]);

            if (j != 0 && is_in_range(b) && mod_idx(b) >= mod_idx(base_idx))
                tiles_in_range.push_back(&m_tile_map[b]);

            if (i == 0)
                continue;

            if (is_in_range(c) && mod_idx(c) <= mod_idx(base_idx))
                tiles_in_range.push_back(&m_tile_map[c]);

            if (j != 0 && is_in_range(d) && mod_idx(d) <= mod_idx(base_idx))
                tiles_in_range.push_back(&m_tile_map[d]);
        }
    }

    return tiles_in_range;
}

std::vector<int> world::get_path_indicees(const std::vector<tile*>& path_tiles)
{
    std::vector<int> indicees;
    indicees.reserve(path_tiles.size());

    for (auto& tile : path_tiles) 
    {
        indicees.push_back(tile->m_tile_map_idx);
    }

    return indicees;
}

std::vector<int> world::path_to_target(const std::shared_ptr<creature> creature_1, const std::shared_ptr<creature> creature_2)
{
    auto path_tiles = path_to_target(idx_to_tile(creature_1->m_current_position), idx_to_tile(creature_2->m_current_position));
    return get_path_indicees(path_tiles);
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

void world::animal_make_wander_step(std::shared_ptr<creature>& animal)
{
    // Animal has reached its target
    if (animal->m_path_to_current_target.size() == 1)
    {
        animal->m_current_state = STATE::REST;
        animal->m_steps_since_last_rest = 0;
    }

    double range = ((double)animal->m_speed * 0.5) / 100;

    int terrain_bias;

    if (animal->terrain_type() == PROPERTIES::LANDBEWOHNER)
    {
        terrain_bias = m_terrain_bias_land.find(m_tile_map[animal->m_current_position].m_terrain_type)->second;
    }

    else
    {
        terrain_bias = m_terrain_bias_water.find(m_tile_map[animal->m_current_position].m_terrain_type)->second;
    }

    double effective_range = range / terrain_bias;
    
    // No path available
    if (animal->m_path_to_current_target.size() <= 0)
        return;

    // Next tile is in y direction
    if (mod_idx(animal->m_current_position) == mod_idx(animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]))
    {

        if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.y >= animal->m_position_y)
        {
            animal->m_position_y += effective_range;

            if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.y <= animal->m_position_y)
            {
                m_tile_map[animal->m_current_position].delete_creature_from_tile(animal);
                animal->m_current_position = animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1];
                m_tile_map[animal->m_current_position].add_creature_to_tile(animal);
                animal->m_health -= 2;
                animal->m_path_to_current_target.pop_back();
            }
        }

        else
        {
            animal->m_position_y -= effective_range;

            if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.y >= animal->m_position_y)
            {
                m_tile_map[animal->m_current_position].delete_creature_from_tile(animal);
                animal->m_current_position = animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1];
                m_tile_map[animal->m_current_position].add_creature_to_tile(animal);
                animal->m_health -= 2;
                animal->m_path_to_current_target.pop_back();
            }
        }
    }

    else 
    {
        if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.x >= animal->m_position_x)
        {
            animal->m_position_x += effective_range;

            if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.x <= animal->m_position_x)
            {
                m_tile_map[animal->m_current_position].delete_creature_from_tile(animal);
                animal->m_current_position = animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1];
                m_tile_map[animal->m_current_position].add_creature_to_tile(animal);
                animal->m_position_x = m_tile_map[animal->m_current_position].m_pos.x;
                animal->m_position_y = m_tile_map[animal->m_current_position].m_pos.y;
                animal->m_health -= 2;
                animal->m_path_to_current_target.pop_back();
            }
        }

        else
        {
            animal->m_position_x -= effective_range;

            if (m_tile_map[animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1]].m_pos.x >= animal->m_position_x)
            {
                m_tile_map[animal->m_current_position].delete_creature_from_tile(animal);
                animal->m_current_position = animal->m_path_to_current_target[animal->m_path_to_current_target.size() - 1];
                m_tile_map[animal->m_current_position].add_creature_to_tile(animal);
                animal->m_health -= 2;
                animal->m_path_to_current_target.pop_back();
            }
        }
    }
}