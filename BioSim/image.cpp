#include "image.hpp"

/////////////////////////////////////////////////////////////
//
//
//  Class: invalid_image_format
//
//
/////////////////////////////////////////////////////////////

const char* invalid_image_format::what() const noexcept
{
	return "image data has invalid format";
}

/////////////////////////////////////////////////////////////
//
//
//  Class: corrupted_image_data
//
//
/////////////////////////////////////////////////////////////

const char* corrupted_image_data::what() const noexcept
{
	return "image data is corrupted";
}

/////////////////////////////////////////////////////////////
//
//
//  Class: image
//
//
/////////////////////////////////////////////////////////////

image::image(const std::string& image_path)
{
	load_image(image_path, this->m_pixel_data, this->m_header);
}

image::~image()
{
}

void image::load_image(const std::string& path, std::vector<char>& pixel_data, TGA_HEADER& header)
{
	std::ifstream ifs(path, std::ios::in | std::ios::binary);

	if (ifs.is_open())
	{
		/* read header */
		ifs.read(				&header.id_length,			sizeof(header.id_length));
		ifs.read(				&header.colour_map_type,	sizeof(header.colour_map_type));
		ifs.read(				&header.image_type,			sizeof(header.image_type));

		ifs.read((char*)		&header.first_entry,		sizeof(header.first_entry));
		ifs.read((char*)		&header.num_entries,		sizeof(header.num_entries));
		ifs.read(				&header.bits_per_entry,		sizeof(header.bits_per_entry));

		ifs.read((char*)		&header.x_origin,			sizeof(header.x_origin));
		ifs.read((char*)		&header.y_origin,			sizeof(header.y_origin));
		ifs.read((char*)		&header.width,				sizeof(header.width));
		ifs.read((char*)		&header.height,				sizeof(header.height));
		ifs.read(				&header.bits_per_pixel,		sizeof(header.bits_per_pixel));
		ifs.read(				&header.descriptor,			sizeof(header.descriptor));

		/* throws exception in case of invalid format */
		check_image_format(header.id_length,
			header.colour_map_type,
			header.first_entry,
			header.num_entries,
			header.image_type,
			header.x_origin,
			header.y_origin,
			header.bits_per_pixel
		);

		/* calculate space needed for the image */
		int image_data_size = header.width * header.height * (header.bits_per_pixel / 8);
		pixel_data.resize(image_data_size);

		/* read actual image data */
		ifs.read(pixel_data.data(), image_data_size);

		/* throws exception if image_data_size differs from count of bytes read actually */
		if (ifs)
			std::cout << "-> [DEBUG]: Image read" << std::endl;
		else
			throw corrupted_image_data();

		ifs.close();
	}

	else
	{
		throw file_not_found();
	}
}

void image::save_image_fs(const std::string& path, image& image)
{
	std::string full_path = path + OUTPUT_FILE_NAME_;

	std::ofstream ofs;
	ofs.open(full_path, std::ios::out | std::ios::binary);

	if (ofs.is_open())
	{
		/* Header consists of bytes and words (= two bytes), so we have to convert the words into two bytes by divide/modulo 256
	   to be able to write bytewise */
		char header_bytewise[HEADER_SIZE_BYTES_] = {						(char)image.m_header.id_length,
																			(char)image.m_header.colour_map_type,
																			(char)image.m_header.image_type,
																			(char)(image.m_header.first_entry % 256),
																			(char)(image.m_header.first_entry / 256),
																			(char)(image.m_header.num_entries % 256),
																			(char)(image.m_header.num_entries / 256),
																			(char)image.m_header.bits_per_entry,
																			(char)(image.m_header.x_origin % 256),
																			(char)(image.m_header.x_origin / 256),
																			(char)(image.m_header.y_origin % 256),
																			(char)(image.m_header.y_origin / 256),
																			(char)(image.m_header.width % 256),
																			(char)(image.m_header.width / 256),
																			(char)(image.m_header.height % 256),
																			(char)(image.m_header.height / 256),
																			(char)image.m_header.bits_per_pixel, 
																			(char)image.m_header.descriptor
		};

		/* Write header to file */
		for (auto& byte : header_bytewise)	
			ofs << byte;

		for (size_t i = 0; i < (size_t)image.m_header.width * (size_t)image.m_header.height; i++)
		{
			for (size_t b = 0; b < 4; b++)
			{
				ofs << image.m_pixel_data[(i * BITS_PER_CHANNEL_) + (b % BITS_PER_CHANNEL_)];
			}
		}
	}

	std::cout << "-> [DEBUG]: File saved" << std::endl;
}

void image::save_image(const std::string& path, image& image)
{
	std::string full_path = path + OUTPUT_FILE_NAME_;

	FILE* fp = NULL;
	fopen_s(&fp, full_path.c_str(), "wb");

	if (fp == NULL)
		throw invalid_file_path();

	/* Header consists of bytes and words (= two bytes), so we have to convert the words into two bytes by divide/modulo 256
	   to be able to write bytewise */
	char header_bytewise[HEADER_SIZE_BYTES_] = {    (char)image.m_header.id_length,
													(char)image.m_header.colour_map_type,
													(char)image.m_header.image_type,
													(char)(image.m_header.first_entry % 256),
													(char)(image.m_header.first_entry / 256),
													(char)(image.m_header.num_entries % 256),
													(char)(image.m_header.num_entries / 256),
													(char)image.m_header.bits_per_entry,
													(char)(image.m_header.x_origin % 256),
													(char)(image.m_header.x_origin / 256),
													(char)(image.m_header.y_origin % 256),
													(char)(image.m_header.y_origin / 256),
													(char)(image.m_header.width % 256),
													(char)(image.m_header.width / 256),
													(char)(image.m_header.height % 256),
													(char)(image.m_header.height / 256),
													(char)image.m_header.bits_per_pixel,
													(char)image.m_header.descriptor
	};

	fwrite(header_bytewise, sizeof(header_bytewise), 1, fp);

	for (size_t  i = 0; i < (size_t)image.m_header.width * (size_t)image.m_header.height; i++)
	{
		for (size_t b = 0; b < 4; b++)
		{
			fputc(image.m_pixel_data[(i * BITS_PER_CHANNEL_) + (b % BITS_PER_CHANNEL_)], fp);
		}
	}

	std::cout << "-> [DEBUG]: File saved" << std::endl;

	fclose(fp);
}

void image::check_image_format( const char&		id_length, 
								const char&		colour_map_type, 
								const short&	first_entry, 
								const short&	num_entries, 
								const char&		image_type, 
								const short&	x_origin, 
								const short&	y_origin, 
								const char&		bits_per_pixel)
{

	if (id_length			!= 0x0	||
		colour_map_type		!= 0x0	||
		first_entry			!= 0x0	||
		num_entries			!= 0x0	||
		image_type			!= 0x2	||
		x_origin			!= 0x0	||
		y_origin			!= 0x0	||
		bits_per_pixel		!= 0x20
		)
	{
		throw invalid_image_format();
	}
}

std::vector<std::shared_ptr<image>> image::load_images(const std::string& directory_path)
{
	std::vector<std::shared_ptr<image>> images;

	for (auto& entry : std::filesystem::directory_iterator(directory_path))
		images.push_back(std::make_shared<image>(entry.path().string()));

	return images;
}

const std::string image::OUTPUT_FILE_NAME_ = "output.tga";