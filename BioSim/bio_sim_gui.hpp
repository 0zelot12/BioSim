#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>

#include "ui_bio_sim_gui.h"
#include "bio_sim_presenter.hpp"
#include "image.hpp"
#include "QSimulationScene.hpp"
#include "QSimulationTile.hpp"

class bio_sim_gui : public QMainWindow
{
    Q_OBJECT

public:
    bio_sim_gui(QWidget *parent = Q_NULLPTR);

    std::vector<std::shared_ptr<image>>                  m_tga_terrain_images;
    std::vector<std::shared_ptr<TERRAIN_IMAGE>>          m_terrain_images;
    std::vector<std::shared_ptr<image>>                  m_tga_creature_images;
    std::vector<std::shared_ptr<CREATURE_IMAGE>>         m_creature_images;

public slots:
    void on_ctrl_start_btn_clicked();
    void on_ctrl_stop_btn_clicked();
    void on_ctrl_step_btn_clicked();
    void on_place_creature_btn_clicked();
    void on_creature_choice_box_currentIndexChanged(int index);

private:
    Ui::bio_sim_guiClass                                 m_ui;
    bio_sim_presenter                                    m_presenter;
    QSimulationScene                                     m_simulation_scene;                        /* Scene connceted to simulation_area */

    void fill_creature_selection(int idx);                                                          /* Fills values of creature selection into the form */
};
