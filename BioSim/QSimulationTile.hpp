#pragma once

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

#include "world.hpp"
#include "image.hpp"

class QSimulationTile : public QGraphicsPixmapItem
{
    using QGraphicsPixmapItem::QGraphicsPixmapItem;

public:
    int             m_terrain_type_idx;
    QPixmap*        m_terrain_image; /* Unmodified terrain data */
};