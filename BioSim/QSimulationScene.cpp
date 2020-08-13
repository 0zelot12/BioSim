#include "QSimulationScene.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: QSimulationScene
//
//
/////////////////////////////////////////////////////////////

bool QSimulationScene::add_new_creature(creature* new_creature)
{
    QSimulationTile* current_tile = (QSimulationTile*)this->m_last_cursor_item;

    if (!current_tile)
    {
        return false;
    }

    new_creature->m_current_position = current_tile->m_tile_map_idx;

    /* Check if creature type and terrain type match */
    if (new_creature->m_is_land_creature)
    {
        if (current_tile->m_terrain_type_idx == 0 || current_tile->m_terrain_type_idx == 4)
        {
            return false;
        }
    }
    else 
    {
        if (current_tile->m_terrain_type_idx != 0 && current_tile->m_terrain_type_idx != 4)
        {
            return false;
        }
    }

    current_tile->m_creatures_on_tile.push_back(new_creature);
    this->draw_creatures(true);
    return true;
}

void QSimulationScene::draw_creatures(bool is_cursor)
{
    QSimulationTile* current_tile = (QSimulationTile*)this->m_last_cursor_item;

    QPixmap tile_with_creature = current_tile->m_current_image_data;
    QPainter painter(&tile_with_creature);

    for (int i = 0; i < current_tile->m_creatures_on_tile.size(); i++)
    {
        painter.drawPixmap(0, 0, *(current_tile->m_creatures_on_tile[i]->m_creature_image));
    }

    if (is_cursor)
    {
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
            QSimulationTile* clicked_tile       = (QSimulationTile*)object_clicked;
            QSimulationTile* unclicked_tile     = (QSimulationTile*)this->m_last_cursor_item;

            if(!m_last_path_tiles.empty())
                clear_path_tiles(this->m_last_path_tiles);

            QPixmap new_pixmap = clicked_tile->pixmap();
            QPixmap cursor_pixmap(".\\graphics\\environment\\cursor\\cursor.tga");

            if (unclicked_tile)
            {
                unclicked_tile->setPixmap(unclicked_tile->m_current_image_data); //???
                this->draw_creatures(false);
            }

            QPainter painter(&new_pixmap);

            painter.drawPixmap(0, 0, cursor_pixmap);
            clicked_tile->setPixmap(new_pixmap);

            /* Remeber tile that has the cursor */
            this->m_last_cursor_item = object_clicked;
        }
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        QPointF         point_clicked   = mouseEvent->scenePos();

        QGraphicsItem*  object_clicked  = this->itemAt(point_clicked, QTransform());

        if (object_clicked && object_clicked != this->m_last_cursor_item)
        {
            QSimulationTile* clicked_tile = (QSimulationTile*)object_clicked;
            QSimulationTile* unclicked_tile = (QSimulationTile*)this->m_last_cursor_item;

            QPixmap new_pixmap = clicked_tile->pixmap();
            QPixmap old_pixmap = *unclicked_tile->m_terrain_image;

            QPixmap cursor_pixmap   (".\\graphics\\environment\\cursor\\cursor.tga");
            QPixmap path_pixmap     (".\\graphics\\environment\\path\\path.tga");

            if (unclicked_tile)
            {
                //unclicked_tile->setPixmap(unclicked_tile->m_current_image_data);
                //this->draw_creatures(false);

                clear_path_tiles(this->m_last_path_tiles);

                auto path_tiles = this->m_model->m_world.path_to_target(clicked_tile, unclicked_tile);

                if (!path_tiles.empty())
                {
                    for (auto& tile : path_tiles)
                    {
                        auto pixmap = tile->pixmap();
                        QPainter path_painter(&pixmap);
                        path_painter.drawPixmap(0, 0, path_pixmap);
                        tile->setPixmap(pixmap);
                    }

                    this->m_last_path_tiles = path_tiles;
                }

                else
                {
                    QMessageBox msg_box;
                    msg_box.setText("No path available!");
                    msg_box.exec();
                    this->m_last_cursor_item = nullptr;
                    return;
                }
            }

            QPainter cursor_painter(&old_pixmap);

            cursor_painter.drawPixmap(0, 0, cursor_pixmap);
            unclicked_tile->setPixmap(old_pixmap);

            ///* Remeber tile that has the cursor */
            //this->m_last_cursor_item = object_clicked;
        }

    }
}

void QSimulationScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
}

void QSimulationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
}

void QSimulationScene::repaint_map()
{
    for (auto item : this->m_model->m_world.m_tile_map)
    {
        QSimulationTile* tile = (QSimulationTile*)item;
        auto pixmap = tile->pixmap();
        QPainter path_painter(&pixmap);
        path_painter.drawPixmap(0, 0, *tile->m_terrain_image);
        tile->setPixmap(pixmap);
    }
}

void QSimulationScene::clear_path_tiles(std::vector<QSimulationTile*> last_path_tiles)
{
    for (auto tile : last_path_tiles)
    {
        auto pixmap = tile->pixmap();
        QPainter path_painter(&pixmap);
        path_painter.drawPixmap(0, 0, *tile->m_terrain_image);
        tile->setPixmap(pixmap);
    }
}
