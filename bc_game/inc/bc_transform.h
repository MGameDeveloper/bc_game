#pragma once

#include <glm/glm.hpp>

class bc_transform
{
private:
	glm::vec3 m_position;
	glm::vec3 m_rotate_axis;
	glm::vec3 m_scale;
	float     m_rotate_angle;

public:
	bc_transform();
	~bc_transform();
	bc_transform(const bc_transform&)             = delete;
	bc_transform(const bc_transform&&)            = delete;
	bc_transform& operator=(const bc_transform&)  = delete;
	bc_transform& operator=(const bc_transform&&) = delete;

	glm::vec3 get_position();
	glm::vec3 get_scale();
	glm::vec3 get_rotate_axis();
	float     get_rotate_angle();

	void set_position(const glm::vec3& position);
	void set_scale(const glm::vec3& scale);
	
	void translate(const glm::vec3& pos);
	void rotate(const glm::vec3& axis, float angle);
	void scale(const glm::vec3& scale);

	glm::mat4 get_model_matrix();
};
