#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class SimulationScene : public QGraphicsScene
{
protected:
    void mousePressEvent    (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent     (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent  (QGraphicsSceneMouseEvent* mouseEvent) override;

};