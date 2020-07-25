#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPathItem>

#include "QSimulationTile.hpp"

class bio_sim_gui;

class QSimulationScene : public QGraphicsScene
{
protected:
    void mousePressEvent    (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent     (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent  (QGraphicsSceneMouseEvent* mouseEvent) override;
private:
    QGraphicsItem* m_last_cursor_item = nullptr;
};