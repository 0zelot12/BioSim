#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPathItem>

#include "QSimulationTile.hpp"
#include "creature.hpp"

/**
*************************************************************************
* @class QSimulationScene
*
* Custom class for handling the simulation graphics
*************************************************************************/
class QSimulationScene : public QGraphicsScene
{
public:
    /* Add new creature to simualtion */
    bool add_new_creature(creature new_creature);
    /* Draw all creatures on a given tile */
    void draw_creatures(bool is_cursor);
protected:
    void mousePressEvent    (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent     (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent  (QGraphicsSceneMouseEvent* mouseEvent) override;
private:
    /* Last tile that was selected with the cursor */
    QGraphicsItem* m_last_cursor_item = nullptr;
};