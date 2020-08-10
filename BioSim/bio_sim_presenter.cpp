#include "bio_sim_presenter.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: bio_sim_presenter
//
//
/////////////////////////////////////////////////////////////

bio_sim_presenter::bio_sim_presenter()
try : model(PATH_TO_CREATURE_TABLE)
{

}
catch (const std::exception& e)
{
	throw file_not_found();
}

const std::vector<std::shared_ptr<creature_type>>& bio_sim_presenter::m_creature_types()
{
	return model.m_creature_types;
}

const std::string bio_sim_presenter::PATH_TO_CREATURE_TABLE = ".\\graphics\\creatures\\CreatureTable.txt";