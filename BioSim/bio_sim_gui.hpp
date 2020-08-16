#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>

#include <map>
#include <iostream> 
#include <iterator> 

#include "ui_bio_sim_gui.h"
#include "QSimulationScene.hpp"
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

    // Terrain images in TGA format 
    std::vector<std::shared_ptr<image>> m_tga_terrain_images;

    std::shared_ptr<QPixmap> m_cursor_pixmap;
    std::shared_ptr<QPixmap> m_path_pixmap;

    // Maps a terrain type to its original image data
    std::map<TERRAIN_TYPE, QPixmap> m_terrain_type_to_pixmap;

    // Maps a creature to its original image data
    std::map<std::shared_ptr<creature_type>, QPixmap> m_creature_type_to_pixmap;

public slots:

    void on_ctrl_start_btn_clicked();
    void on_ctrl_stop_btn_clicked();
    void on_ctrl_step_btn_clicked();
    void on_place_creature_btn_clicked();
    void on_creature_choice_box_currentIndexChanged(int index);

private:

    // Qt GUI class 
    Ui::bio_sim_guiClass m_ui;

    // Presenter get data from the model and passes it to the GUI 
    bio_sim_presenter m_presenter;

    // Scene connceted to simulation_area 
    QSimulationScene m_simulation_scene;   

    // Fills values of creature selection into the form 
    void fill_creature_selection(int idx);                                                          
};
