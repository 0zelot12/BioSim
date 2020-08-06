#pragma once

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

#include "image.hpp"
#include "creature.hpp"

class QSimulationTile : public QGraphicsPixmapItem
{
    using QGraphicsPixmapItem::QGraphicsPixmapItem;

public:
    /* Terrain type of the tile */
    int                     m_terrain_type_idx;
    /* Tile map index */    
    int                     m_tile_map_idx;
    /* All creatures on this tile */
    std::vector<creature*>  m_creatures_on_tile;
    /* Unmodified terrain data */
    QPixmap*                m_terrain_image;            
    /* Image data currently shown */
    QPixmap                 m_current_image_data;

    QSimulationTile* m_last_tile = nullptr;
    int g_distance = 0, f_distance = 0, h_value = 0;

};