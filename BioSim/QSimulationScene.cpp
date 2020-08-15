#include "QSimulationScene.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: QSimulationScene
//
//
/////////////////////////////////////////////////////////////

void QSimulationScene::draw_tile(int position, bool is_cursor)
{
    auto current_tile = m_model->m_world.m_tile_map[position];
    auto creatures = current_tile.m_creatures_on_tile;

    auto terrain_pixmap = m_terrain_type_to_pixmap->find(current_tile.m_terrain_type)->second;
    
    QPixmap new_pixmap = terrain_pixmap;

    QPainter painter(&new_pixmap);
    for (auto& creature : creatures)
    {
        painter.drawPixmap(0, 0, *creature->m_creature_image);
    }

    if (is_cursor)
    {
        QPixmap cursor_pixmap(".\\graphics\\environment\\cursor\\cursor.tga");
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

        if (clicked_index >= 0 && clicked_index < m_model->m_world.m_tile_map.size() - 1)
        {
            if(!m_last_path_tiles.empty())
                reset_tiles(this->m_last_path_tiles);

            if (m_last_cursor_index >= 0)
            {
                draw_tile(m_last_cursor_index, false);
            }

            draw_tile(clicked_index, true);

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

            QPixmap path_pixmap(".\\graphics\\environment\\path\\path.tga");

            if (m_last_cursor_index >= 0)
            {
                reset_tiles(this->m_last_path_tiles);

                auto path_tiles = this->m_model->m_world.path_to_target(idx_to_tile(m_last_cursor_index), tile_clicked);

                if (!path_tiles.empty())
                {
                    for (auto tile : path_tiles)
                    {
                        auto pixmap = get_pixmap(*tile);
                        QPainter path_painter(&pixmap);
                        path_painter.drawPixmap(0, 0, path_pixmap);
                        set_pixmap(pixmap, *tile);
                    }

                    this->m_last_path_tiles = path_tiles;
                }

                else
                {

                    draw_tile(m_last_cursor_index, false);
                    this->m_last_cursor_index = -1;

                    QMessageBox msg_box;
                    msg_box.setText("No path available!");
                    msg_box.exec();
                    return;
                }
            }

            draw_tile(m_last_cursor_index, true);
        }
    }
}

void QSimulationScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
}

void QSimulationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
}

void QSimulationScene::reset_tiles(std::vector<tile*> last_path_tiles)
{
    for (auto tile : last_path_tiles)
    {
        auto pixmap = get_pixmap(*tile);
        draw_tile(tile->m_tile_map_idx, false);
    }
}

int QSimulationScene::get_tile_map_idx(QPointF point_clicked)
{
    int x_val = ((int)point_clicked.x()) / 32;
    int y_val = ((int)point_clicked.y()) / 32;

    int result = x_val + y_val * 128;

    return result;
}

QPixmap QSimulationScene::get_pixmap(const tile& tile)
{
    return m_terrain_type_to_pixmap->find(m_model->m_world.m_tile_map[tile.m_tile_map_idx].m_terrain_type)->second;
}

void QSimulationScene::set_pixmap(QPixmap& pixmap, const tile& tile)
{
    int position = tile.m_tile_map_idx;
    auto tiles = this->items();
    QGraphicsPixmapItem* scene_item = (QGraphicsPixmapItem*)tiles[position];
    scene_item->setPixmap(pixmap);
}

tile* QSimulationScene::idx_to_tile(int idx)
{
    return &(m_model->m_world.m_tile_map[idx]);
}
