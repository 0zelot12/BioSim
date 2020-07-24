#include "common_exceptions.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: file_not_found
//
//
/////////////////////////////////////////////////////////////

const char* file_not_found::what() const noexcept
{
	return "file not found";
}

/////////////////////////////////////////////////////////////
//
//
//  Class: invalid_console_argument
//
//
/////////////////////////////////////////////////////////////

const char* invalid_console_argument::what() const noexcept
{
	return "invalid console argument";
}

const char* invalid_file_path::what() const noexcept
{
	return "specified filepath is invalid";
}
