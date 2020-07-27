#pragma once

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

#include "world.hpp"
#include "image.hpp"
#include "creature.hpp"

class QSimulationTile : public QGraphicsPixmapItem
{
    using QGraphicsPixmapItem::QGraphicsPixmapItem;

public:
    int                     m_terrain_type_idx;
    std::vector<creature>   m_creatures_on_tile;
    QPixmap*                m_terrain_image;            /* Unmodified terrain data */
    QPixmap                 m_current_image_data;
};