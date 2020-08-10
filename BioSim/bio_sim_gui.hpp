#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>

#include "ui_bio_sim_gui.h"
#include "QSimulationScene.hpp"
#include "QSimulationTile.hpp"
#include "bio_sim_presenter.hpp"
#include "image.hpp"
#include "creature.hpp"
#include "common_exceptions.hpp"

/**
*************************************************************************
* @class bio_sim_gui
*
* Class handles user interface EIGENTLICH PRESENTER
*************************************************************************/
class bio_sim_gui : public QMainWindow
{
    Q_OBJECT

public:
    bio_sim_gui(QWidget *parent = Q_NULLPTR);

    /* Terrain images in TGA format */
    std::vector<std::shared_ptr<image>>                  m_tga_terrain_images;
    /* Terrain images in custom format */
    std::vector<std::shared_ptr<TERRAIN_IMAGE>>          m_terrain_images;
    /* Creature images in TGA format */
    std::vector<std::shared_ptr<image>>                  m_tga_creature_images;
    /* Creature images in custom format */
    std::vector<std::shared_ptr<CREATURE_IMAGE>>         m_creature_images;

public slots:
    void on_ctrl_start_btn_clicked();
    void on_ctrl_stop_btn_clicked();
    void on_ctrl_step_btn_clicked();
    void on_place_creature_btn_clicked();
    void on_creature_choice_box_currentIndexChanged(int index);

private:
    /* Qt GUI class */
    Ui::bio_sim_guiClass                                 m_ui;
    /* Presenter get data from the model and passes it to the GUI */
    bio_sim_presenter                                    m_presenter;
    /* Scene connceted to simulation_area */
    QSimulationScene                                     m_simulation_scene;   

    /* Fills values of creature selection into the form */
    void fill_creature_selection(int idx);                                                          
};
