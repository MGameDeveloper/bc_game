#pragma once

#include <glm/glm.hpp>
#include "bc_ints.h"
#include "bc_transform.h"

struct bc_shader;
struct bc_camera;

class bc_sprite
{
private:
	friend class bc_render;

	struct bc_vertex
	{
		glm::vec4 color;
		glm::vec3 pos;
		glm::vec2 uv;
		float     clut_id          = 0;
		float     texture_id       = 0;
		float     b_use_clut       = 0;
		float     text_texture_idx = 0;
		glm::vec2 text_texutre_uv;
	};

	bc_vertex v[4];

public:
	bc_transform transform;

public:
	bc_sprite();
	~bc_sprite();
	bc_sprite(const bc_sprite&)             = delete;
	bc_sprite(const bc_sprite&&)            = delete;
	bc_sprite& operator=(const bc_sprite&)  = delete;
	bc_sprite& operator=(const bc_sprite&&) = delete;

	void set_color   (const glm::vec4& color);
	void set_texture (u32 texture_id);
	void set_uv      (const glm::vec2& uv);
	void set_clut    (u32 clut_id);
	
	void set_text_texture(u32 texture_id);
	void set_text_texture_uv(const glm::vec2& uv);
};


class bc_render
{
private:
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

	void set_shader();
	void set_camera();

	void draw(bc_sprite& sprite);
};