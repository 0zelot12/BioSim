#include "bio_sim_gui.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	try
	{
		QApplication a(argc, argv);
		bio_sim_gui w;
		w.show();
		return a.exec();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
