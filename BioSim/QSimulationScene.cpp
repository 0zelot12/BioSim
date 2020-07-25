#include "QSimulationScene.hpp"

void QSimulationScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        QPointF         point_clicked   = mouseEvent->scenePos();
        QGraphicsItem*  object_clicked  = this->itemAt(point_clicked, QTransform());

        /* Make sure object exists and is not already marked with cursor */
        if (object_clicked && object_clicked != this->m_last_cursor_item)
        {
            QSimulationTile* new_item       = (QSimulationTile*)object_clicked;
            QSimulationTile* old_item       = (QSimulationTile*)this->m_last_cursor_item;

            QPixmap new_pixmap = new_item->pixmap();
            QPixmap cursor_pixmap("C:\\Users\\lauri\\source\\repos\\BioSim\\BioSim\\graphics\\environment\\cursor\\cursor.tga");

            if (old_item)
                old_item->setPixmap(*old_item->m_terrain_image);

            QPainter painter(&new_pixmap);
            painter.drawPixmap(0, 0, cursor_pixmap);
            new_item->setPixmap(new_pixmap);

            /* Remeber tile that has the cursor */
            this->m_last_cursor_item = object_clicked;
        }
    }
}

void QSimulationScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
}

void QSimulationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
}
