#include "bio_sim_gui.hpp"

bio_sim_gui::bio_sim_gui(QWidget* parent)
try : QMainWindow(parent),
m_tga_terrain_images(image::load_images(".\\graphics\\environment\\terrain"))
{
    _ui.setupUi(this);

    /* Populate the creature selection with data */
    auto creatures = _presenter.m_creature_types();
    for (int i = 0; i < creatures.size(); i++)
    {
        /* Store the vector index of the creature as data */
        _ui.creature_choice_box->addItem(QString::fromStdString(creatures[i]->name()),
            QVariant::fromValue(i));
    }

    /* Put the first creature_type in
    the list as initial value */
    fill_creature_selection(0);


    /* Convert from image to TERRAIN_IMAGE */
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

    /* Configure QGraphicsView */
    _ui.simulation_area->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    _ui.simulation_area->setCacheMode(QGraphicsView::CacheBackground);
    _ui.simulation_area->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    _ui.simulation_area->setInteractive(false);

    /* Configure QGraphicsScene */
    _simulation_scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    /* Fill scene with pixelmaps */
    for (int i = 0; i < _presenter.model.WORLD_HEIGHT; i++)
    {
        for (int j = 0; j < _presenter.model.WORLD_WIDTH; j++)
        {
            int idx = world::terrain_type_to_int(_presenter.model.m_world.terrain_map[world::coordinate_to_index(i, j, _presenter.model.WORLD_HEIGHT)]);
            QGraphicsPixmapItem* pixmap_item = new QGraphicsPixmapItem(m_terrain_images[idx]->q_pixmap);
            _simulation_scene.addItem(pixmap_item);
            pixmap_item->setPos(m_terrain_images[0]->tga_image->height() * j,
                m_terrain_images[0]->tga_image->width() * i);
        }
    }

    // Show scene
    _ui.simulation_area->setScene(&_simulation_scene);
    _ui.simulation_area->show();

    _ui.statusBar->showMessage("Statusleiste");
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
    QMessageBox msg_box;
    msg_box.setText("Place button clicked");
    msg_box.exec();
}

void bio_sim_gui::on_creature_choice_box_currentIndexChanged(int index)
{
    fill_creature_selection(index);
}

void bio_sim_gui::fill_creature_selection(int idx)
{
    /* Display correct name */
    _ui.creature_choice_box->setCurrentIndex(idx);

    auto creatures = _presenter.m_creature_types();
    _ui.lifespan_edit->setText(QString::number(creatures[idx]->lebensdauer()));
    _ui.strength_edit->setText(QString::number(creatures[idx]->staerke()));
    _ui.speed_edit->setText(QString::number(creatures[idx]->geschwindigkeit()));

    /* A creature_type can have multiple properties seperated by whitespaces */
    QString properties_str = "";
    for (auto m_property : creatures[idx]->eigenschaften_list())
    {
        properties_str += QString::fromStdString(attributes::property_to_str(m_property)) + " ";
    }
    _ui.properties_edit->setText(properties_str);
}
