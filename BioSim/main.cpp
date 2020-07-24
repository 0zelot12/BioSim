#include "bio_sim_gui.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bio_sim_gui w;
    w.show();
    return a.exec();
}
