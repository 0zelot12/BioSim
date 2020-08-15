#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPathItem>

#include "creature.hpp"
#include "bio_sim_model.hpp"
#include "world.hpp"

/**
*************************************************************************
* @class QSimulationScene
*
* Custom class for handling the simulation graphics
*************************************************************************/

class QSimulationScene : public QGraphicsScene
{
public:

    // Draw tile with all contents 
    void draw_tile(int position, bool is_cursor);

    // Returns the current index of the cursor
    int get_current_cursor_position();

    //TODO: In methode auslagern
    std::map<TERRAIN_TYPE, QPixmap>* m_terrain_type_to_pixmap;

    //TODO: In Konstruktor auslagern
    bio_sim_model* m_model = nullptr;

protected:

    void mousePressEvent    (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent     (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent  (QGraphicsSceneMouseEvent* mouseEvent) override;

private:

    // Resets tiles to its original terrain graphic 
    void reset_tiles(std::vector<tile*> tiles_to_redraw);

    // Remember tiles that were used by the last path 
    std::vector<tile*> m_last_path_tiles;

    // Last tile that was selected with the cursor 
    int m_last_cursor_index = -1;

    // Returns the linear vector index for a point clicked on the GUI
    int get_tile_map_idx(QPointF point_clicked);

    // Returns original pixmap for the the tile
    QPixmap get_pixmap(const tile& tile);

    // Sets the given pixmap as image of the specified tile
    void set_pixmap(QPixmap& pixmap, const tile& tile);

    // Returns pointer to tile at a given index
    tile* idx_to_tile(int idx);
};