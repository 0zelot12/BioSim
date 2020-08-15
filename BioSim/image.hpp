#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>


#include "common_exceptions.hpp"

/**
*************************************************************************
* @class invalid_image_format
*
* Throws if the image is in an unsupported format
*************************************************************************/

class invalid_image_format : public std::exception
{
	virtual const char* what() const noexcept override;
};

/**
*************************************************************************
* @class corrupted_image_data
*
* Throws if fewer bytes were read, than specified in the header

*************************************************************************/

class corrupted_image_data : public std::exception
{
	virtual const char* what() const noexcept override;
};

/**
*************************************************************************
* @struct TGA_HEADER
*
* Defines datatype for an image in TGA format
* The header consists of 18 bytes (char = 1 byte; short = 2 bytes)
*************************************************************************/

struct TGA_HEADER
{
	// Length of id string 
	char id_length;
	char colour_map_type;
	char image_type;
	// First entry of color map 
	short first_entry;
	// Number of entries in the color map 
	short num_entries;
	// Bits per entry in the color map 
	char bits_per_entry;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char bits_per_pixel;
	char descriptor;
};

/**
*************************************************************************
* @class image
*
* Defines datatype for an image in TGA format
* The header consists of 18 bytes (char = 1 byte; short = 2 bytes)
*************************************************************************/

class image
{
public:

	image(const std::string& image_path);

	// Holds the image data -NOT NEEDED -> pointer->getdata()
	std::vector<char> m_pixel_data;

	// Saves image object to the specified path
	static void save_image(const std::string& path, image& image);

	// Saves image object to the specified path using ofilstream
	static void save_image_fs(const std::string& path, image& image);

	// Expose data without possibility of changing 
	const std::vector<char>& pixel_data() const { return m_pixel_data; }

	// Loads all image files from the specified directory 
	static std::vector<std::shared_ptr<image>> load_images(const std::string& directory_path);

	const char width()	{ return m_header.width; };
	const char height()	{ return m_header.width; };	

private:

	// Contains metainfo of the image 
	TGA_HEADER m_header;

	// Checks whether the format is supported, throws invalid_image_format in the latter case 
	static void check_image_format(const char& id_length,
		const char& colour_map_type,
		const short& first_entry,
		const short& num_entries,
		const char& image_type,
		const short& x_origin,
		const short& y_origin,
		const char& bits_per_pixel
	);

	// Loads image from the TGA file at the specified path 
	static void load_image(const std::string& path, std::vector<char>& pixel_data, TGA_HEADER& header);

	// Filename for saved image (only for debugging) 
	static const std::string OUTPUT_FILE_NAME_;
	static const int BITS_PER_CHANNEL_ = 4;
	static const int HEADER_SIZE_BYTES_ = 18;
}; 

/**
*************************************************************************
* @struct CREATURE_IMAGE
*
* Custom datatype for handling creature images in Qt datatype
*************************************************************************/
struct CREATURE_IMAGE
{
	std::shared_ptr<image>  tga_image;
	QByteArray              q_bytes;
	QImage                  q_image;
	QPixmap                 q_pixmap;
};