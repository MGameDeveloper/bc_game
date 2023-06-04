#include <glm/gtc/matrix_transform.hpp>
#include "../inc/bc_camera.h"

bc_camera::bc_camera(float view_width, float view_height, float near_plane, float far_plane)
{
	m_proj = glm::ortho(0.f, view_width, view_height, 0.f, near_plane, far_plane);
	m_view = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	m_view_size = glm::vec2(view_width, view_height);
}

glm::mat4 bc_camera::get_pv_matrix()
{
	return (m_proj * m_view);
}

glm::vec2 bc_camera::get_view_size()
{
	return m_view_size;
}