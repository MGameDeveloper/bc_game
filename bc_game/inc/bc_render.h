#pragma once

#include <glm/glm.hpp>
#include "bc_ints.h"
#include "bc_transform.h"

struct bc_shader;
struct bc_camera;
struct bc_texture;

struct bc_color
{
	glm::vec4 color;
	
	bc_color(u8 r, u8 g, u8 b, u8 a)
	{
		static float f = 1.f / 255.f;
		color = glm::vec4(r * f, g * f, b * f, a * f);
	}
};

class bc_render
{
private:
	struct bc_vertex
	{
		glm::vec4 color;
		glm::vec3 pos;
		glm::vec2 uv;
		float     clut_id            = 0;
		float     texture_id         = 0;
		float     b_text_use_texture = 0;
		float     text_texture_id    = 0;
		glm::vec2 text_texture_uv;
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

	bc_camera *m_camera = NULL;
	bc_shader *m_shader = NULL;
	bc_canvas *m_canvas = NULL;
	u32        m_vao    = 0;
	u32        m_vbo    = 0;
	u32        m_ebo    = 0;

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

	void draw_sprite(bc_transform* trans, 
		const glm::vec2& uv_pos, 
		const glm::vec2& uv_size,
		bc_texture* texture,
		u32 clut_id,
		const bc_color& color = bc_color(255, 255, 255, 255));
	
	void draw_text(bc_transform* trans, 
		const glm::vec2& uv_pos, 
		const glm::vec2& uv_size,
		bc_texture* font,
		u32 clut_id, 
		const bc_color& color = bc_color(255, 255, 255, 255),
		bc_texture* texture = NULL,
		const glm::vec2& texture_uv = glm::vec2(0.f));

	void clear(u8 r, u8 g, u8 b, u8 a);
	void submit();

	static void init();
	static void show_memory_consumtion();
};