#pragma once

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

#include "creature.hpp"

class QSimulationTile : public QGraphicsPixmapItem
{
    using QGraphicsPixmapItem::QGraphicsPixmapItem;

public:
    // Terrain type of the tile
    unsigned int m_terrain_type_idx;

    // Tile map index  
    int m_tile_map_idx;

    // All creatures on this tile 
    std::vector<creature*> m_creatures_on_tile;

    // Pointer to Unmodified terrain data
    QPixmap* m_terrain_image;          

    // Image data currently shown
    QPixmap m_current_image_data;

    // Pointer to last simulation tile in a path of the A*-algorithm
    QSimulationTile* m_predecessor = nullptr;

    // Indicats whether a tile is located on the edge of the map
    bool m_is_left_edge = false, m_is_right_edge = false;

    /* Distances needed for A*-algorithm */
    int m_g_distance  = 0, m_f_distance  = 0, m_h_distance  = 0;
};