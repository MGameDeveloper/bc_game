#pragma once

#include <glm/glm.hpp>

class bc_camera
{
private:
	glm::mat4 m_proj      = glm::mat4(1.f);
	glm::mat4 m_view      = glm::mat4(1.f);
	glm::vec2 m_view_size = glm::vec2(256, 224);

public:
	bc_camera()                             = default;
	~bc_camera()                            = default;
	bc_camera(const bc_camera&)             = delete;
	bc_camera(const bc_camera&&)            = delete;
	bc_camera& operator=(const bc_camera&)  = delete;
	bc_camera& operator=(const bc_camera&&) = delete;

	bc_camera(float view_width, float view_height, float near_plane, float far_plane);
	glm::mat4 get_pv_matrix();
	glm::vec2 get_view_size();
};