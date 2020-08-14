#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsPathItem>

#include "QSimulationTile.hpp"
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

    /* Add new creature to simulation */
    bool add_new_creature   (creature* new_creature);
    /* Draw all creatures on the last active tile */
    void redraw_items_on_last_tile     (bool is_cursor);
    /* Draw all creatures on the specified tile */
    void draw_creatures      (QSimulationTile* tile, bool is_cursor);

    /* !!Initialize!! */
    bio_sim_model* m_model = nullptr;

protected:

    void mousePressEvent    (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent     (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent  (QGraphicsSceneMouseEvent* mouseEvent) override;



private:

    /* Repaints all tiles on the map with its content */
    void repaint_map();
    /* Resets tiles to its original terrain graphic */
    void reset_tiles(std::vector<QSimulationTile*> last_path_tiles);

    /* Remember tiles that were used by the last path */
    std::vector<QSimulationTile*>   m_last_path_tiles;
    /* Last tile that was selected with the cursor */
    QGraphicsItem*                  m_last_cursor_item = nullptr;
};