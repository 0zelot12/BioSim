#include "QSimulationScene.hpp"

void QSimulationScene::add_new_creature(creature new_creature)
{
    QSimulationTile* current_tile = (QSimulationTile*)this->m_last_cursor_item;
    current_tile->m_creatures_on_tile.push_back(new_creature);
    this->draw_creatures(true);
}

void QSimulationScene::draw_creatures(bool is_cursor)
{
    QSimulationTile* current_tile = (QSimulationTile*)this->m_last_cursor_item;

    QPixmap tile_with_creature = current_tile->m_current_image_data;
    QPainter painter(&tile_with_creature);
    for (int i = 0; i < current_tile->m_creatures_on_tile.size(); i++)
    {
        painter.drawPixmap(0, 0, *current_tile->m_creatures_on_tile[i].m_creature_image);
    }

    if (is_cursor) {
        QPixmap cursor_pixmap(".\\graphics\\environment\\cursor\\cursor.tga");
        painter.drawPixmap(0, 0, cursor_pixmap);
    }

    current_tile->setPixmap(tile_with_creature);
}

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

            QPixmap new_pixmap = new_item->m_current_image_data;
            QPixmap cursor_pixmap(".\\graphics\\environment\\cursor\\cursor.tga");

            if (old_item)
            {

                old_item->setPixmap(old_item->m_current_image_data);
                this->draw_creatures(false);

            }

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
