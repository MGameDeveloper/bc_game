#pragma once

#include <glm/glm.hpp>

struct bc_transform
{
	glm::vec3 m_position;
	glm::vec3 m_rotate_axis;
	glm::vec3 m_scale;
	float     m_rotate_angle;

	void translate(const glm::vec3& pos);
	void rotate(const glm::vec3& axis, float angle);
	void scale(const glm::vec3& scale);

	glm::mat4 get_model_matrix();
};
