#include <glm/gtc/matrix_transform.hpp>
#include "../inc/bc_transform.h"


void bc_transform::translate(const glm::vec3& pos)
{
	m_position = pos;
}

void bc_transform::rotate(const glm::vec3& axis, float angle)
{
	m_rotate_axis  = axis;
	m_rotate_angle = angle;
}

void bc_transform::scale(const glm::vec3& scale)
{
	m_scale = scale;
}

glm::mat4 bc_transform::get_model_matrix()
{
	glm::mat4 m = glm::mat4(1.f);

	m = glm::translate(m, m_position);
	if (m_rotate_angle > 0.f)
		m = glm::rotate(m, glm::radians(m_rotate_angle), m_rotate_axis);
	m = glm::scale(m, m_scale);

	return m;
}