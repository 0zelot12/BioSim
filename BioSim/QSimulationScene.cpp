#include "QSimulationScene.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: QSimulationScene
//
//
/////////////////////////////////////////////////////////////

void QSimulationScene::draw_tile(int position, bool is_cursor, bool is_path)
{
    auto current_tile = &m_model->m_world.m_tile_map[position];
    auto creatures = current_tile->m_creatures_on_tile;

    auto terrain_pixmap = m_terrain_type_to_pixmap->find(current_tile->m_terrain_type)->second;
    QPixmap new_pixmap = terrain_pixmap;

    QPainter painter(&new_pixmap);
    for (auto creature_ptr : creatures)
    {
        auto creature_pixmap = m_creature_type_to_pixmap->find(creature_ptr->m_type)->second;
        painter.drawPixmap(0, 0, creature_pixmap);
    }

    if (is_path)
    {
        QPixmap path_pixmap = this->m_path_pixmap;
        painter.drawPixmap(0, 0, path_pixmap);
    }

    if (is_cursor)
    {
        QPixmap cursor_pixmap = this->m_cursor_pixmap;
        painter.drawPixmap(0, 0, cursor_pixmap);
    }

    auto scene_items = this->items();
    QGraphicsPixmapItem* scene_item = (QGraphicsPixmapItem*)scene_items[position];
    scene_item->setPixmap(new_pixmap);
}

int QSimulationScene::get_current_cursor_position()
{
    return m_last_cursor_index;
}

void QSimulationScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        QPointF point_clicked = mouseEvent->scenePos();

        int clicked_index = get_tile_map_idx(point_clicked);

        if (clicked_index >= 0 && clicked_index < m_model->m_world.m_tile_map.size())
        {
            if(!m_last_path_tiles.empty())
                draw_tiles(this->m_last_path_tiles);

            if (m_last_cursor_index >= 0)
            {
                draw_tile(m_last_cursor_index, false, false);
            }

            draw_tile(clicked_index, true, false);

            // Remeber tile that has the cursor 
            this->m_last_cursor_index = clicked_index;
        }
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        QPointF point_clicked   = mouseEvent->scenePos();

        tile* tile_clicked = &m_model->m_world.m_tile_map[get_tile_map_idx(point_clicked)];

        if (tile_clicked->m_tile_map_idx != m_last_cursor_index)
        {
            if (m_last_cursor_index < 0)
                return;

            if (m_last_cursor_index >= 0)
            {
                draw_tiles(this->m_last_path_tiles);

                auto path_tiles = this->m_model->m_world.path_to_target(idx_to_tile(m_last_cursor_index), tile_clicked);

                if (!path_tiles.empty())
                {
                    for (auto& tile : path_tiles)
                    {
                        draw_tile(tile->m_tile_map_idx, false, true);
                    }

                    this->m_last_path_tiles = path_tiles;
                }

                else
                {

                    draw_tile(m_last_cursor_index, false, false);
                    this->m_last_cursor_index = -1;

                    QMessageBox msg_box;
                    msg_box.setText("No path available!");
                    msg_box.exec();
                    return;
                }
            }

            draw_tile(m_last_cursor_index, true, false);
        }
    }
}

void QSimulationScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
}

void QSimulationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
}

void QSimulationScene::draw_tiles(std::vector<tile*> tiles)
{
    for (auto tile : tiles)
    {
        draw_tile(tile->m_tile_map_idx, false, false);
    }
}

int QSimulationScene::get_tile_map_idx(QPointF point_clicked)
{
    int x_val = ((int)point_clicked.x()) / 32;
    int y_val = ((int)point_clicked.y()) / 32;

    int result = x_val + y_val * this->m_model->WORLD_WIDTH_TILES;

    return result;
}

tile* QSimulationScene::idx_to_tile(int idx)
{
    return &(m_model->m_world.m_tile_map[idx]);
}
