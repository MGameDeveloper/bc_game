#pragma once

#include <stdio.h>
#include <stdint.h>

// most of the fags will create the file if
// doesn't exists except for the one commented
enum file_access_flag_e
{
	// if no file exists it returns null
	FILE_READ, 

	// if no file exists it returns null
	FILE_READ_BINARY,

	// if no file exists it returns null
	FILE_READ_WRITE,

	// if no file exists it returns null
	FILE_READ_WRITE_BINARY,
	
	FILE_WRITE,
	FILE_WRITE_BINARY,
	FILE_APPEND,
	FILE_APPEND_BINARY,
	FILE_WRITE_READ,
	FILE_WRITE_READ_BINARY,
	FILE_READ_APPEND,
	FILE_READ_APPEND_BINARY,
};

struct file_s
{
	uint64_t    size = 0;
	const char* data = NULL;
};

struct file_handle_s
{
	FILE* handle;
	file_access_flag_e access_flag;
};

extern "C"
{
	file_s io_loadfile    (const char* path);
	bool   io_unloadfile  (file_s* file);
	bool   io_isfile_valid(file_s* file);

	file_handle_s io_open_file    (const char* filename, file_access_flag_e access_flag);
	void          io_close_file   (file_handle_s* file);
	void          io_write_to_file(file_handle_s* file, const char* text);
	bool          io_isfile_handle_valid(file_handle_s* handle);
};
