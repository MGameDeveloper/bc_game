#include <glad/glad.h>
#include "../inc/bc_window.h"
#include "../inc/bc_log.h"

static void local_window_error_callback(int error, const char* desc)
{
	bc_log::error("[ GLFW ]: %s", desc);
}

bc_window::bc_window(const char* title, const glm::vec2& size, u32 gl_major_version, u32 gl_minor_version)
{
	static bool glfw_initiated = false;
	if (!glfw_initiated)
	{
		if (!glfwInit())
		{
			bc_log::error("[ bc_window::create( %s ) ]: glfw init failed");
			return;
		}

		glfwSetErrorCallback(local_window_error_callback);
		glfw_initiated = true;
	}

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor_version);

	window = glfwCreateWindow(size.x, size.y, title, NULL, NULL);
	if (!window)
	{
		bc_log::error("[ bc_window( %s ) ]: creation failed");
		return;
	}

	this->title = title;

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

bc_window::~bc_window()
{
	glfwDestroyWindow(window);
}


void bc_window::set_size(const glm::vec2& size)
{
	glfwSetWindowSize(window, size.x, size.y);
}

void bc_window::set_title(const char* title)
{
	glfwSetWindowTitle(window, title);
}

glm::vec2 bc_window::get_size()
{
	i32 w, h;
	glfwGetWindowSize(window, &w, &h);

	return glm::vec2(w, h);
}

const char* bc_window::get_title()
{
	return title;
}

bool bc_window::is_closing()
{
	return glfwWindowShouldClose(window);
}

void bc_window::swap_buffers()
{
	glfwSwapBuffers(window);
}

void bc_window::poll_events()
{
	glfwPollEvents();
}

void bc_window::activate_as_current_context()
{
	glfwMakeContextCurrent(window);
}

void bc_window::onkey_callback(GLFWkeyfun key_callback)
{
	glfwSetKeyCallback(window, key_callback);
}