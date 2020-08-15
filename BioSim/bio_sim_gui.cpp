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
m_tga_terrain_images(image::load_images(".\\graphics\\environment\\terrain"))
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

    // Load creature images 
    std::vector<std::shared_ptr<image>> land_creatures_tga_images = image::load_images(".\\graphics\\environment\\land");
    std::vector<std::shared_ptr<image>> water_creatures_tga_images = image::load_images(".\\graphics\\environment\\wasser");

    m_tga_creature_images.reserve(land_creatures_tga_images.size() + water_creatures_tga_images.size());

    m_tga_creature_images.insert(m_tga_creature_images.end(),
        water_creatures_tga_images.begin(),
        water_creatures_tga_images.end()
    );

    // Combine land and water images 
    m_tga_creature_images.insert(m_tga_creature_images.end(),
        land_creatures_tga_images.begin(),
        land_creatures_tga_images.end()
    );

    for (int i = 0; i < m_tga_creature_images.size(); i++)
    {
        auto name = this->m_presenter.m_creature_types().at(i)->name();
        std::shared_ptr<QPixmap> pixmap = std::make_shared<QPixmap>(QPixmap::fromImage(
            QImage(
                (unsigned char*)m_tga_creature_images[i]->m_pixel_data.data(),
                m_tga_creature_images[i]->width(),
                m_tga_creature_images[i]->height(),
                QImage::Format_ARGB32
            )));
        m_creature_type_to_pixmap.insert(std::pair<std::string, std::shared_ptr<QPixmap>>(name, pixmap));
    }



    // Convert from image to CREATURE_IMAGE
    for (int i = 0; i < m_tga_creature_images.size(); i++)
    {
        CREATURE_IMAGE img;

        img.tga_image = m_tga_creature_images[i];

        QByteArray byte_array;
        for (auto& byte : img.tga_image->pixel_data())
        {
            byte_array.append(byte);
        }
        img.q_bytes = byte_array;
        /* AAAAAUFPASSEN */
        /* Move Konstruktor */
        img.q_image = QImage((unsigned char*)img.q_bytes.data(),
            img.tga_image->height(),
            img.tga_image->height(),
            QImage::Format_ARGB32
        ).mirrored();
        img.q_pixmap = QPixmap::fromImage(img.q_image);

        m_creature_images.push_back(std::make_shared<CREATURE_IMAGE>(img));
    }

    // Load the terrain images and convert them to QPixmap, then put them to the map
    for (int i = 0; i < m_tga_terrain_images.size(); i++)
    {
        TERRAIN_TYPE type = world::int_to_terrain_type(i);
        QPixmap pixmap = QPixmap::fromImage(
            QImage(
                (unsigned char*)m_tga_terrain_images[i]->m_pixel_data.data(),
                m_tga_terrain_images[i]->width(),
                m_tga_terrain_images[i]->height(),
                QImage::Format_ARGB32
            ));
        m_terrain_type_to_pixmap.insert(std::pair<TERRAIN_TYPE, QPixmap>(type, pixmap));

    }

    // Fill scene with pixelmaps
    for (uint32_t i = 0; i < m_presenter.model.WORLD_HEIGHT_TILES; i++)
    {
        for (uint32_t j = 0; j < m_presenter.model.WORLD_WIDTH_TILES; j++)
        {
            int index = world::coordinate_to_index(j, i, m_presenter.model.WORLD_HEIGHT_TILES);
            
            QPixmap pixmap = m_terrain_type_to_pixmap.find(m_presenter.model.m_world.m_tile_map[index].m_terrain_type)->second;

            QGraphicsPixmapItem* pixmap_item = m_simulation_scene.addPixmap(pixmap);

            pixmap_item->setPos(m_tga_terrain_images[0]->height() * j, m_tga_terrain_images[0]->width() * i);
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
    
    // Show scene
    m_ui.simulation_area->setScene(&m_simulation_scene);
    m_ui.simulation_area->show();

    m_ui.statusBar->showMessage("Statusleiste");
}
catch (const std::exception& e)
{
    QMessageBox messageBox;
    messageBox.critical(0, "Error", "An error has occured! ");
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
    QMessageBox msg_box;
    msg_box.setText("Step button clicked");
    msg_box.exec();
}

void bio_sim_gui::on_place_creature_btn_clicked()
{
    // AUSLAGERN IN WORLD
    // Get selected creature type
    int new_creature_index = m_ui.creature_choice_box->currentIndex();
    auto type = this->m_presenter.m_creature_types().at(new_creature_index);

    std::shared_ptr<creature> new_creature = std::make_shared<creature>(type->staerke(),
        type->geschwindigkeit(),
        m_simulation_scene.get_current_cursor_position(),
        type->name(),
        type->eigenschaften_list(),
        &m_creature_images.at(new_creature_index)->q_pixmap);

    bool success = this->m_presenter.model.m_world.add_creature(new_creature);
    if (!success)
    {
        QMessageBox msg_box;
        msg_box.setText("You can't place this here!");
        msg_box.exec();
        return;
    }

    m_simulation_scene.draw_tile(new_creature->m_current_position, true);
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
    m_ui.lifespan_edit->setText  (QString::number(creatures[idx]->lebensdauer    ()));
    m_ui.strength_edit->setText  (QString::number(creatures[idx]->staerke        ()));
    m_ui.speed_edit->   setText  (QString::number(creatures[idx]->geschwindigkeit()));

    // A creature_type can have multiple properties seperated by whitespaces 
    QString properties_str = "";
    for (auto m_property : creatures[idx]->eigenschaften_list())
    {
        properties_str += QString::fromStdString(attributes::property_to_str(m_property)) + " ";
    }
    m_ui.properties_edit->setText(properties_str);
}