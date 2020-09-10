#include "bio_sim_gui.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: bio_sim_gui
//
//
/////////////////////////////////////////////////////////////

bio_sim_gui::bio_sim_gui(QWidget* parent)
try : QMainWindow(parent),
m_tga_terrain_images(image::load_terrain_images(".\\graphics\\environment\\terrain"))
{
    m_ui.setupUi(this);

    // Populate the creature selection with data
    auto creatures = m_presenter.m_creature_types();
    for (int i = 0; i < creatures.size(); i++)
    {
        // Store the vector index of the creature as data 
        m_ui.creature_choice_box->addItem(QString::fromStdString(creatures[i]->name()),
            QVariant::fromValue(i));
    }

    //Put the first creature_type in the list as initial value 
    fill_creature_selection(0);

    // Get pixmap of the cursor
    std::shared_ptr<image> cursor_tga = image::load_image(".\\graphics\\environment\\cursor\\cursor.tga");

    // Get pixmap of path symbol
    std::shared_ptr<image> path_tga = image::load_image(".\\graphics\\environment\\path\\path.tga");

    // Load the creature images and convert them to QPixmap, then put them to the map
    for (int i = 0; i < this->m_presenter.m_creature_types().size(); i++)
    {
        image image_tga = image(".\\graphics\\environment\\" + this->m_presenter.m_creature_types().at(i)->image_path());
        QPixmap pixmap = QPixmap::fromImage(
            QImage(
                (unsigned char*)image_tga.pixel_data().data(),
                image_tga.width(),
                image_tga.height(),
                QImage::Format_ARGB32
            ).mirrored());

        m_creature_type_to_pixmap.insert(std::pair<std::shared_ptr<creature_type>,
            QPixmap>(this->m_presenter.m_creature_types().at(i), pixmap));
    }

    // Load the terrain images and convert them to QPixmap, then put them to the map
    for (int i = 0; i < m_tga_terrain_images.size(); i++)
    {
        TERRAIN_TYPE type = world::int_to_terrain_type(i);
        QPixmap pixmap = QPixmap::fromImage(
            QImage(
                (unsigned char*)m_tga_terrain_images[i]->pixel_data().data(),
                m_tga_terrain_images[i]->width(),
                m_tga_terrain_images[i]->height(),
                QImage::Format_ARGB32
            ).mirrored());
        m_terrain_type_to_pixmap.insert(std::pair<TERRAIN_TYPE, QPixmap>(type, pixmap));
    }

    // Fill scene
    for (uint32_t i = 0; i < m_presenter.model.WORLD_HEIGHT_TILES; i++)
    {
        for (uint32_t j = 0; j < m_presenter.model.WORLD_WIDTH_TILES; j++)
        {
            int index = world::coordinate_to_index(j, i, m_presenter.model.WORLD_HEIGHT_TILES);
            
            QPixmap pixmap = m_terrain_type_to_pixmap.find(m_presenter.model.m_world.m_tile_map[index].m_terrain_type)->second;

            QGraphicsPixmapItem* pixmap_item = m_simulation_scene.addPixmap(pixmap);

            pixmap_item->setPos(32 * j, 32 * i);
        }
    }

    // Configure QGraphicsView
    m_ui.simulation_area->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_ui.simulation_area->setCacheMode(QGraphicsView::CacheBackground);
    m_ui.simulation_area->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    m_ui.simulation_area->setInteractive(true);

    // Configure QGraphicsScene
    m_simulation_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    m_simulation_scene.m_model = &this->m_presenter.model;
    m_simulation_scene.m_terrain_type_to_pixmap = &this->m_terrain_type_to_pixmap;
    m_simulation_scene.m_creature_type_to_pixmap = &this->m_creature_type_to_pixmap;

    m_simulation_scene.m_cursor_pixmap = QPixmap::fromImage(
        QImage((unsigned char*)cursor_tga->pixel_data().data(),
            cursor_tga->width(),
            cursor_tga->height(),
            QImage::Format_ARGB32
        ).mirrored());

    m_simulation_scene.m_path_pixmap = QPixmap::fromImage(QImage(
            (unsigned char*)path_tga->pixel_data().data(),
            path_tga->width(),
            path_tga->height(),
            QImage::Format_ARGB32
        ).mirrored());
    
    // Show scene
    m_ui.simulation_area->setScene(&m_simulation_scene);
    m_ui.simulation_area->show();

    m_ui.statusBar->showMessage("Statusleiste");
}
catch (const std::exception& e)
{
    QMessageBox messageBox;
    messageBox.critical(0, "Error", e.what());
}


void bio_sim_gui::on_ctrl_start_btn_clicked()
{
        QMessageBox msg_box;
        msg_box.setText("Start button clicked");
        msg_box.exec();
}

void bio_sim_gui::on_ctrl_stop_btn_clicked()
{
    QMessageBox msg_box;
    msg_box.setText("Stop button clicked");
    msg_box.exec();
}

void bio_sim_gui::on_ctrl_step_btn_clicked()
{
    this->m_presenter.model.m_world.simulation_step();
}

void bio_sim_gui::on_place_creature_btn_clicked()
{
    // Get selected creature type
    auto type = this->m_presenter.m_creature_types().at(m_ui.creature_choice_box->currentIndex());

    // Put it to simualation, show error if not successfull
    bool success = this->m_presenter.model.m_world.add_creature(type, m_simulation_scene.get_current_cursor_position());
    if (!success)
    {
        QMessageBox msg_box;
        msg_box.setText("You can't place this here!");
        msg_box.exec();
        return;
    }

    // Draw the tile with the creature + the cursor on it
    m_simulation_scene.draw_tile(m_simulation_scene.get_current_cursor_position(), true, false);
}

void bio_sim_gui::on_creature_choice_box_currentIndexChanged(int index)
{
    fill_creature_selection(index);
}

void bio_sim_gui::fill_creature_selection(int idx)
{
    // Display correct name 
    m_ui.creature_choice_box->setCurrentIndex(idx);

    auto creatures = m_presenter.m_creature_types();
    m_ui.lifespan_edit->setText(QString::number(creatures[idx]->life_span()));
    m_ui.strength_edit->setText(QString::number(creatures[idx]->strength()));
    m_ui.speed_edit->setText(QString::number(creatures[idx]->speed()));

    // A creature_type can have multiple properties seperated by whitespaces 
    QString properties_str = "";
    for (auto m_property : creatures[idx]->property_list())
    {
        properties_str += QString::fromStdString(attributes::property_to_str(m_property)) + " ";
    }
    m_ui.properties_edit->setText(properties_str);
}