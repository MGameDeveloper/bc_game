#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../inc/bc_time.h"

bc_time::bc_time()
{
	start_time = glfwGetTime();
}

void bc_time::update()
{
	dt            = glfwGetTime() - start_time;
	elapsed_time += dt;
	start_time    = glfwGetTime();
}