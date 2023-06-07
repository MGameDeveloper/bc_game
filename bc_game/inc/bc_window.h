#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "bc_ints.h"

class bc_window
{
private:
	GLFWwindow* window = NULL;
	const char* title  = NULL;

public:
	static void poll_events();
	
	bc_window(const char* title, const glm::vec2& size, u32 gl_major_version = 4, u32 gl_minor_version = 6);
	~bc_window();
	bc_window(const bc_window&)             = delete;
	bc_window(const bc_window&&)            = delete;
	bc_window& operator=(const bc_window&)  = delete;
	bc_window& operator=(const bc_window&&) = delete;

	void        set_size(const glm::vec2& size);
	void        set_title(const char* title);
	glm::vec2   get_size();
	const char* get_title();

	bool is_closing();
	void swap_buffers();
	void activate_as_current_context();

	void register_input(class bc_input* input);
};