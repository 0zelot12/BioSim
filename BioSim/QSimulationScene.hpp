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

    bool path_error = false;

    /* Add new creature to simualtion */
    bool add_new_creature   (creature* new_creature);
    /* Draw all creatures on a given tile */
    void draw_creatures     (bool is_cursor);


    bio_sim_model* m_model = nullptr;


protected:

    void mousePressEvent    (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent     (QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent  (QGraphicsSceneMouseEvent* mouseEvent) override;

    /* Last tile that was selected with the cursor */
    QGraphicsItem* m_last_cursor_item = nullptr;
    std::vector<QSimulationTile*> m_last_path_tiles;

private:

    /* Repaints all tiles on the map with its content */
    void repaint_map();
    void clear_path_tiles(std::vector<QSimulationTile*> last_path_tiles);
};