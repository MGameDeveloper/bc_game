#pragma once

#include <glm/glm.hpp>
#include "bc_ints.h"
#include "bc_transform.h"

struct bc_shader;
struct bc_camera;

class bc_render
{
private:
	struct bc_vertex
	{
		glm::vec4 color;
		glm::vec3 pos;
		glm::vec2 uv;
		float     clut_id = 0;
		float     texture_id = 0;
		float     b_use_clut = 0;
		float     text_texture_idx = 0;
		glm::vec2 text_texutre_uv;
	};

	struct bc_sprite
	{
		bc_vertex v[4];
	};

	struct bc_canvas
	{
		bc_sprite *sprites = NULL;
		i32        count   = 0;
		i32        idx     = 0;
	};

	bc_camera* m_camera;
	bc_shader* m_shader;
	bc_canvas  m_canvas;
	u32        m_vao;
	u32        m_vbo;
	u32        m_ebo;

public:
	bc_render(u32 max_sprite_count);
	~bc_render();
	bc_render()                             = default;
	bc_render(const bc_render&)             = delete;
	bc_render(const bc_render&&)            = delete;
	bc_render& operator=(const bc_render&)  = delete;
	bc_render& operator=(const bc_render&&) = delete;

	void set_shader(bc_shader* shader);
	void set_camera(bc_camera* camera);

	void draw(bc_transform* trans)
};