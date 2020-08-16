#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <memory>

/**
*************************************************************************
* @class file_not_found
*
* Throws if the specified path doesn't exist
*************************************************************************/
class file_not_found : public std::exception
{
	virtual const char* what() const noexcept override;
};

/**
*************************************************************************
* @class invalid_file_path	
*
* Throws if the specified path is invalid
*************************************************************************/
class invalid_file_path : public std::exception
{
	virtual const char* what() const noexcept override;
};

/**
*************************************************************************
* @class invalid_console_argument
*
* Throws if an invalid argument was entered into the console
*************************************************************************/
class invalid_console_argument : public std::exception
{
	virtual const char* what() const noexcept override;
};