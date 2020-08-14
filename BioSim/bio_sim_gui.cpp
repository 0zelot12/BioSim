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

    // Load creature images 
    std::vector<std::shared_ptr<image>> land_creatures_tga_images   = image::load_images(".\\graphics\\environment\\land");
    std::vector<std::shared_ptr<image>> water_creatures_tga_images  = image::load_images(".\\graphics\\environment\\wasser");

    m_tga_creature_images.reserve(land_creatures_tga_images.size() + water_creatures_tga_images.size());

    m_tga_creature_images.insert(   m_tga_creature_images.end(),
                                    water_creatures_tga_images.begin(),
                                    water_creatures_tga_images.end()
    );

    // Combine land and water images 
    m_tga_creature_images.insert(   m_tga_creature_images.end(),
                                    land_creatures_tga_images.begin(), 
                                    land_creatures_tga_images.end()
    );



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
        img.q_image = QImage((unsigned char*)   img.q_bytes.data(),
                                                img.tga_image->height(),
                                                img.tga_image->height(),
                                                QImage::Format_ARGB32
        ).mirrored();
        img.q_pixmap = QPixmap::fromImage(img.q_image);

        m_creature_images.push_back(std::make_shared<CREATURE_IMAGE>(img));
    }

    // Populate the creature selection with data
    auto creatures = m_presenter.m_creature_types();
    for (int i = 0; i < creatures.size(); i++)
    {
        /* Store the vector index of the creature as data */
        m_ui.creature_choice_box->addItem(QString::fromStdString(creatures[i]->name()),
                                          QVariant::fromValue(i));
    }

    //Put the first creature_type in the list as initial value 
    fill_creature_selection(0);

    // Convert from image to TERRAIN_IMAGE
    for (int i = 0; i < 6; i++)
    {
        TERRAIN_IMAGE img;

        img.tga_image = m_tga_terrain_images[i];

        QByteArray byteArr;
        for (auto& byte : img.tga_image->pixel_data())
        {
            byteArr.append(byte);
        }
        img.q_bytes = byteArr;
        img.q_image = QImage((unsigned char*)img.q_bytes.data(),
            img.tga_image->height(),
            img.tga_image->height(),
            QImage::Format_ARGB32
        );
        img.q_pixmap = QPixmap::fromImage(img.q_image);

        m_terrain_images.push_back(std::make_shared<TERRAIN_IMAGE>(img));
    }

    // Configure QGraphicsView
    m_ui.simulation_area->setAlignment           (Qt::AlignLeft | Qt::AlignTop);
    m_ui.simulation_area->setCacheMode           (QGraphicsView::CacheBackground);
    m_ui.simulation_area->setViewportUpdateMode  (QGraphicsView::SmartViewportUpdate);
    m_ui.simulation_area->setInteractive         (true);

    // Configure QGraphicsScene
    m_simulation_scene.setItemIndexMethod        (QGraphicsScene::NoIndex);
    m_simulation_scene.m_model = &this->m_presenter.model;

    // Fill scene with pixelmaps
    for (uint32_t i = 0; i < m_presenter.model.WORLD_HEIGHT_TILES; i++)
    {
        for (uint32_t j = 0; j < m_presenter.model.WORLD_WIDTH_TILES; j++)
        {
            int idx = world::terrain_type_to_int(m_presenter.model.m_world.m_terrain_map[world::coordinate_to_index(i, j, m_presenter.model.WORLD_HEIGHT_TILES)]);
            QSimulationTile* pixmap_item        = new QSimulationTile(m_terrain_images[idx]->q_pixmap);
            pixmap_item->m_terrain_type_idx     = idx;
            pixmap_item->m_terrain_image        = &m_terrain_images[idx]->q_pixmap;
            pixmap_item->m_current_image_data   = m_terrain_images[idx]->q_pixmap;
            m_presenter.model.m_world.m_tile_map.push_back(pixmap_item);
            pixmap_item->m_tile_map_idx = m_presenter.model.m_world.m_tile_map.size() - 1;

            if (pixmap_item->m_tile_map_idx % m_presenter.model.WORLD_WIDTH_TILES == 0)
            {
                pixmap_item->m_is_left_edge = true;
            }

            if (pixmap_item->m_tile_map_idx % m_presenter.model.WORLD_WIDTH_TILES == 127)
            {
                pixmap_item->m_is_right_edge = true;
            }

            m_simulation_scene.addItem(pixmap_item);
            pixmap_item->setPos(m_terrain_images[0]->tga_image->height()*j,
                                m_terrain_images[0]->tga_image->width() *i
            );
        }
    }

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
    /* Get selected creature type */
    int new_creature_index = m_ui.creature_choice_box->currentIndex();
    auto type = this->m_presenter.m_creature_types().at(new_creature_index);

    /* Check if creature is a land creature */
    bool is_land = new_creature_index < 8 ? false : true;

    /* Instantiate creature */
    creature* new_creature = new creature(  type->staerke(), 
                                            type->geschwindigkeit(), 
                                            -1, 
                                            type->name(), 
                                            type->eigenschaften_list(), 
                                            &m_creature_images.at(new_creature_index)->q_pixmap
    );

    /* Draw creature, show error if it fails */
    bool success = this->m_simulation_scene.add_new_creature(new_creature);
    if (!success)
    {
        QMessageBox msg_box;
        msg_box.setText("You can't place this here!");
        msg_box.exec();
    }

    /* Put instance to into the model */
    this->m_presenter.model.m_world.m_creature_map.push_back(std::make_shared<creature>(*new_creature));
}

void bio_sim_gui::on_creature_choice_box_currentIndexChanged(int index)
{
    fill_creature_selection(index);
}

void bio_sim_gui::fill_creature_selection(int idx)
{
    /* Display correct name */
    m_ui.creature_choice_box->setCurrentIndex(idx);

    auto creatures = m_presenter.m_creature_types();
    m_ui.lifespan_edit->setText  (QString::number(creatures[idx]->lebensdauer    ()));
    m_ui.strength_edit->setText  (QString::number(creatures[idx]->staerke        ()));
    m_ui.speed_edit->   setText  (QString::number(creatures[idx]->geschwindigkeit()));

    /* A creature_type can have multiple properties seperated by whitespaces */
    QString properties_str = "";
    for (auto m_property : creatures[idx]->eigenschaften_list())
    {
        properties_str += QString::fromStdString(attributes::property_to_str(m_property)) + " ";
    }
    m_ui.properties_edit->setText(properties_str);
}
