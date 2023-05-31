#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../inc/bc_log.h"
#include "../inc/bc_ints.h"

#define RED_ANSI    "\x1b[31m"
#define GREEN_ANSI  "\x1b[92m"
#define YELLOW_ANSI "\x1b[93m"
#define CYAN_ANSI   "\x1b[36m"
#define RESET_ANSI  "\x1b[0m"

#define LOG_SIZE 1024

struct log_data_s
{
	FILE* file = NULL;
	const char* file_name = "LOG.txt";

	char log[LOG_SIZE];

	bool  has_new_log       = true;
	bool  is_log_to_file    = true;
	bool  is_log_to_console = true;
};
static log_data_s log_data;



static bool local_init_log_if_not()
{
	static bool has_init = false;
	if (!has_init)
	{
		fopen_s(&log_data.file, log_data.file_name, "w");
		if (!log_data.file)
			return false;
		else
			fclose(log_data.file);


		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD mode;
		if (GetConsoleMode(handle, &mode))
		{
			// to allow console coloring when running exe directly
			// otherwise coloring only works when running from within 
			// visual studio
			SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		}

		has_init = true;
	}

	return true;
}
#define INIT_LOG_IF_NOT()        \
    if(!local_init_log_if_not()) \
        return;


static void local_write_log_to_file(const char* log_type, const char* format, va_list args)
{
	if (log_data.is_log_to_file)
	{
		memset(log_data.log, 0, LOG_SIZE);

		i32 log_type_len = strlen(log_type) - 1;
		if (log_type_len <= 0)
			return;
		else
			memcpy_s(log_data.log, LOG_SIZE, log_type, log_type_len);

		u32 char_count = vsprintf_s(log_data.log + log_type_len, LOG_SIZE, format, args);
		
		u32 total_size = char_count + log_type_len + 1;
		if (total_size >= LOG_SIZE)
			total_size = LOG_SIZE;

		log_data.log[total_size - 1] = '\n';

		fopen_s(&log_data.file, log_data.file_name, "a");
		if (log_data.file)
		{
			fwrite(log_data.log, sizeof(char), total_size, log_data.file);
			fclose(log_data.file);
		}

		log_data.has_new_log = true;
	}
}
#define LOG_TO_FILE(type, format)                  \
    va_list args_1;                                \
    va_start(args_1, format);                      \
    local_write_log_to_file(type, format, args_1); \
    va_end(args_1)


static void local_write_log_to_console(const char* log_type, const char* format, va_list args)
{
	if (log_data.is_log_to_console)
	{
		memset(log_data.log, 0, LOG_SIZE);

		i32 log_type_len = strlen(log_type) - 1;
		if (log_type_len <= 0)
			return;
		else
		{
			if (strcmp(log_type, "[ TEXT ]") == 0)
				log_type_len = 0;
			else
				strcpy_s(log_data.log, LOG_SIZE, log_type);
		}

		

		u32 char_count = vsprintf_s(log_data.log + log_type_len, LOG_SIZE, format, args);

		u32 total_size = char_count + log_type_len + 2;
		if (total_size >= LOG_SIZE)
			total_size = LOG_SIZE;

		log_data.log[total_size - 2] = '\n';
		log_data.log[total_size - 1] = '\0';

		fprintf(stdout, log_data.log);
		fprintf(stdout, RESET_ANSI);
	}
}
#define LOG_TO_CONSOLE(type, format)                  \
    va_list args_2;                                   \
    va_start(args_2, format);                         \
    local_write_log_to_console(type, format, args_2); \
    va_end(args_2)


void bc_log::should_log_to_file(bool is_log_to_file)
{
	log_data.is_log_to_file = is_log_to_file;
}

void bc_log::should_log_to_console(bool is_log_to_console)
{
	log_data.is_log_to_console = is_log_to_console;
}

void bc_log::info(const char* format, ...)
{
	INIT_LOG_IF_NOT();
	LOG_TO_FILE("[ INFO ]:  ", format);
	LOG_TO_CONSOLE(GREEN_ANSI"[ INFO ]:  ", format);
}

void bc_log::warning(const char* format, ...)
{
	INIT_LOG_IF_NOT();
	LOG_TO_FILE("[ WARNING ]:  ", format);
	LOG_TO_CONSOLE(YELLOW_ANSI"[ WARNING ]:  ", format);
}

void bc_log::error(const char* format, ...)
{
	INIT_LOG_IF_NOT();
	LOG_TO_FILE("[ ERROR ]:  ", format);
	LOG_TO_CONSOLE(RED_ANSI"[ ERROR ]:  ", format);
}

void bc_log::trace(const char* format, ...)
{
	INIT_LOG_IF_NOT();
	LOG_TO_FILE("[ TRACE ]:  ", format);
	LOG_TO_CONSOLE(CYAN_ANSI"[ TRACE ]:  ", format);
}

void bc_log::text(const char* format, ...)
{
	INIT_LOG_IF_NOT();
	LOG_TO_FILE("[ TEXT ]", format);
	LOG_TO_CONSOLE("[ TEXT ]", format);
}