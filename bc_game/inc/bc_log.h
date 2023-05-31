#pragma once

struct bc_log
{
	static void should_log_to_file(bool is_log_to_file);
	static void should_log_to_console(bool is_log_to_console);
	static void info   (const char* format, ...);
	static void warning(const char* format, ...);
	static void error  (const char* format, ...);
	static void trace  (const char* format, ...);
	static void text   (const char* format, ...);
};