#pragma once

#include "creature_type.hpp"
#include "image.hpp"
#include "bio_sim_model.hpp"
#include "common_exceptions.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <memory>
#include <QtWidgets/QMainWindow>

/**
*************************************************************************
* @class bio_sim_presenter
*
* Handles communication between model and gui
*************************************************************************/
class bio_sim_presenter
{
public:
	bio_sim_presenter();
	/* Exposes the creature types */
	const std::vector<std::shared_ptr<creature_type>>& m_creature_types();
	/* Model object containing the simulation table */
	bio_sim_model model;
	
private:
	const static std::string PATH_TO_CREATURE_TABLE;
};