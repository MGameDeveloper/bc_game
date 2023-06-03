#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/bc_io.h"
#include "../inc/bc_log.h"


   
static long io_filesize(FILE* file)
{
	fseek(file, 0L, SEEK_END);  
	long file_size = ftell(file);
	fseek(file, 0L, SEEK_SET);  

	if (file_size == -1L)
	{
		bc_log::error("io_filesize failed");
	}

	return file_size + 1;
}

static const char* io_alloc(size_t file_size)
{
	const char* file_content = (const char*)malloc(file_size);
	if (!file_content)
	{
		bc_log::error("io_alloc failed");
		return NULL;
	}

	memset((void*)file_content, 0, file_size);
	return file_content;
}

static bool io_free(file_s* file)
{
	free((void*)file->data);
	file->size = 0;
	return true;
}

file_s io_loadfile(const char* path)
{
	FILE*  file      = NULL;
	size_t fread_ret = 0;
	file_s entire_file;

	if (!path)
	{
		bc_log::error("io_loadfile( %s ) path is not valid", path);
	}
	else
	{
		fopen_s(&file, path, "r"); // change it from rb to r to fix reading shader file issue, make sure that reading audio is still working
		if (!file)
		{
			bc_log::error("io_loadfile( %s ) filed to open the file", path);
		}
		else
		{
			entire_file.size = io_filesize(file);
			entire_file.data = io_alloc(entire_file.size); // we may add streaming instead of loading the entire file into memory
			fread_ret        = fread_s((void*)entire_file.data, entire_file.size, 1, entire_file.size, file);

			fclose(file);
		}	
	}
	
	bc_log::trace("io_loadfile( %s ), %lld bytes read", path, fread_ret);
	return entire_file;
}

bool io_unloadfile(file_s* file)
{
	if (!file)
		return false;

	file->size = 0;
	return io_free(file);
}

bool io_isfile_valid(file_s* file)
{
	return (file->data && file->size > 0);
}




static const char* local_file_access_mode(file_access_flag_e access_flag)
{
	switch (access_flag)
	{
	case FILE_READ:               return "r";
	case FILE_READ_BINARY:        return "rb";
	case FILE_READ_WRITE:         return "r+";
	case FILE_READ_WRITE_BINARY:  return "rb+";
	case FILE_WRITE:              return "w";
	case FILE_WRITE_BINARY:       return "wb";
	case FILE_APPEND:             return "a";
	case FILE_APPEND_BINARY:      return "ab";
	case FILE_WRITE_READ:         return "w+";
	case FILE_WRITE_READ_BINARY:  return "wb+";
	case FILE_READ_APPEND:        return "a+";
	case FILE_READ_APPEND_BINARY: return "ab+";
	}

	return NULL;
}

file_handle_s io_open_file(const char* filename, file_access_flag_e access_flag)
{
	file_handle_s file;
	file.access_flag = access_flag;
	fopen_s(&file.handle, filename, local_file_access_mode(access_flag));
	
	return file;
}

void io_close_file(file_handle_s* file)
{
	if (!file)
		return;

	fclose(file->handle);
}

void io_write_to_file(file_handle_s* file, const char* text)
{
	if (!file)
		return;

	unsigned int txt_size = strlen(text);
	size_t wrtten_size = fwrite(text, 1, txt_size, file->handle);
}

bool io_isfile_handle_valid(file_handle_s* file)
{
	return (file && file->handle);
}