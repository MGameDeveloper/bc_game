#include <glad/glad.h>
#include "../inc/bc_render.h"
#include "../inc/bc_texture.h"
#include "../inc/bc_mem_tracker.h"
#include "../inc/bc_camera.h"
#include "../inc/bc_shader.h"

static bc_memory_tracker_s bc_mem("[ BC_RENDER ]");

static void GLAPIENTRY local_error_callback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* user_param)
{
	bc_log::error("[ GL ]: %s type = 0x%x, severity = 0x%x, message = %s",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, msg);
}

bc_render::bc_render(u32 max_sprite_count)
{
	if (max_sprite_count == 0)
	{
		bc_log::warning("[ bc_render( %d ) ]: invalid param", max_sprite_count);
		return;
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);


	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bc_vertex) * max_sprite_count * 4, NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(bc_vertex), (void*)offsetof(bc_vertex, color));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(bc_vertex), (void*)offsetof(bc_vertex, pos));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(bc_vertex), (void*)offsetof(bc_vertex, uv));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(bc_vertex), (void*)offsetof(bc_vertex, clut_id));
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(bc_vertex), (void*)offsetof(bc_vertex, texture_id));
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(bc_vertex), (void*)offsetof(bc_vertex, b_text_use_texture));
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(bc_vertex), (void*)offsetof(bc_vertex, text_texture_id));
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(bc_vertex), (void*)offsetof(bc_vertex, text_texture_uv));

	// sprite indecies
	u32 index_size = sizeof(u32) * max_sprite_count * 6;
	u32* indecies = (u32*)bc_mem.alloc(index_size);
	if (!indecies)
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
		bc_log::error("[ bc_render( %d ) ]: failed to allocate index buffer", max_sprite_count);
		return;
	}
	else
	{
		bc_mem.zero(indecies, index_size);

		u32 offset = 0;
		for (u32 idx = 0; idx < max_sprite_count * 6; idx += 6)
		{
			indecies[0 + idx] = 0 + offset;
			indecies[1 + idx] = 1 + offset;
			indecies[2 + idx] = 2 + offset;
			indecies[3 + idx] = 0 + offset;
			indecies[4 + idx] = 3 + offset;
			indecies[5 + idx] = 2 + offset;

			offset += 4;
		}
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indecies, GL_STATIC_DRAW);

		bc_mem.dealloc(indecies);
	}

	glBindVertexArray(0);


	// Creating Sprite Canvas
	u32 canvas_size = sizeof(bc_canvas) + (sizeof(bc_sprite) * max_sprite_count);
	m_canvas = (bc_canvas*)bc_mem.alloc( canvas_size );
	if (!m_canvas)
	{
		glDeleteBuffers(1, &m_ebo);
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);

		bc_log::error("[ bc_render( %d ) ]: failed to create bc_canvas", max_sprite_count);
		return;
	}
	else
		bc_mem.zero(m_canvas, canvas_size);

	m_canvas->count   = max_sprite_count;
	m_canvas->idx     = 0;
	m_canvas->sprites = (bc_sprite*)((u8*)m_canvas + sizeof(bc_canvas));
}

bc_render::~bc_render()
{
	glDeleteBuffers(1, &m_ebo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);

	bc_mem.dealloc(m_canvas);
}

void bc_render::set_shader(bc_shader* shader)
{
	m_shader = shader;
}

void bc_render::set_camera(bc_camera* camera)
{
	m_camera = camera;
}

void bc_render::draw_sprite(bc_transform* trans,
	const glm::vec2& uv_pos,
	const glm::vec2& uv_size,
	bc_texture* texture,
	u32 clut_id,
	const bc_color& color)
{
	if (!m_canvas)
	{
		bc_log::error("[ bc_render::draw_sprite( %s ) ]: m_canvas is not valid");
		return;
	}

	if (!trans)
	{
		bc_log::warning("[ bc_render::draw_sprite( ... ) ]: transform param is not valid");
		return;
	}

	if (!texture)
	{
		bc_log::warning("[ bc_render::draw_sprite( ... ) ]: texture param is not valid");
		return;
	}

	if (!m_camera)
	{
		bc_log::error("[ bc_render::draw_sprite( ... ) ]: m_camera is not valid");
		return;
	}

	u32 idx   = m_canvas->idx;
	u32 count = m_canvas->count;

	if (idx < count)
	{
		bc_sprite* s = &m_canvas->sprites[idx++];

		glm::vec2 local_size    = trans->get_size() * trans->get_scale();
		glm::vec2 local_uv_pos  = { uv_pos.x * texture->size_norm().x, uv_pos.y * texture->size_norm().y };
		glm::vec2 local_uv_size = { uv_size.x * texture->size_norm().x, uv_size.y * texture->size_norm().y };

		s->v[0].color = color.color;
		s->v[1].color = color.color;
		s->v[2].color = color.color;
		s->v[3].color = color.color;

		s->v[0].clut_id = clut_id;
		s->v[1].clut_id = clut_id;
		s->v[2].clut_id = clut_id;
		s->v[3].clut_id = clut_id;

		s->v[0].texture_id = texture->idx();
		s->v[1].texture_id = texture->idx();
		s->v[2].texture_id = texture->idx();
		s->v[3].texture_id = texture->idx();

		s->v[0].b_text_use_texture = false;
		s->v[1].b_text_use_texture = false;
		s->v[2].b_text_use_texture = false;
		s->v[3].b_text_use_texture = false;

		s->v[0].text_texture_id = 0.f;
		s->v[1].text_texture_id = 0.f;
		s->v[2].text_texture_id = 0.f;
		s->v[3].text_texture_id = 0.f;

		s->v[0].text_texture_uv = glm::vec2(0.f);
		s->v[1].text_texture_uv = glm::vec2(0.f);
		s->v[2].text_texture_uv = glm::vec2(0.f);
		s->v[3].text_texture_uv = glm::vec2(0.f);

		s->v[0].uv = local_uv_pos;
		s->v[1].uv = glm::vec2(local_uv_pos.x + local_uv_size.x, local_uv_pos.y);
		s->v[2].uv = glm::vec2(local_uv_pos.x + local_uv_size.x, local_uv_pos.y + local_uv_size.y);
		s->v[3].uv = glm::vec2(local_uv_pos.x, local_uv_pos.y + local_uv_size.y);

		glm::mat4 pvm = m_camera->get_pv_matrix() * trans->get_model_matrix();
		//s->v[0].pos = /*pvm */ glm::vec4(trans->get_position(), 1.f);
		//s->v[1].pos = /*pvm */ glm::vec4(trans->get_position() + glm::vec3(trans->get_size().x, 0.f, 0.f), 1.f);
		//s->v[2].pos = /*pvm */ glm::vec4(trans->get_position() + glm::vec3(trans->get_size().x, trans->get_size().y, 0.f), 1.f);
		//s->v[3].pos = /*pvm */ glm::vec4(trans->get_position() + glm::vec3(0.f, trans->get_size().y, 0.f), 1.f);

		// last param of glm::vec4 must be 1.f
		s->v[0].pos = pvm * glm::vec4(0.f, 0.f, 0.f, 1.f);
		s->v[1].pos = pvm * glm::vec4(local_size.x, 0.f, 0.f, 1.f);
		s->v[2].pos = pvm * glm::vec4(local_size.x, local_size.y, 0.f, 1.f);
		s->v[3].pos = pvm * glm::vec4(0.f, local_size.y, 0.f, 1.f);

		m_canvas->idx = idx;
	}
	else
		bc_log::error("[ bc_render::draw_sprite( ... ) ]: m_canvas out of memory");
}

void bc_render::draw_text(bc_transform* trans,
	const glm::vec2& uv_pos,
	const glm::vec2& uv_size,
	bc_texture* font,
	u32 clut_id,
	const bc_color& color,
	bc_texture* texture,
	const glm::vec2& texture_uv)
{

}

void bc_render::clear(u8 r, u8 g, u8 b, u8 a)
{
	static float f = 1.f / 255.f;
	glClearColor(r * f, g * f, b * f, a * f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void bc_render::submit()
{
	if (!m_shader)
	{
		bc_log::warning("[ bc_render::submit() ]: m_shader is NULL");
		return;
	}
	else
		m_shader->bind();

	bc_texture::bind(m_shader);

	glBindVertexArray(m_vao);

	// Updated The Buffer With New Frame Data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bc_sprite) * m_canvas->count, m_canvas->sprites);

	// Issue Drawing Command
	glDrawElements(GL_TRIANGLES, m_canvas->count * 4 * 6, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);

	// clear sprite canvas for the next frame
	bc_mem.zero(m_canvas->sprites, sizeof(bc_sprite) * m_canvas->count);
	m_canvas->idx = 0;
}

void bc_render::init()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(local_error_callback, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void bc_render::show_memory_consumtion()
{
	bc_mem.print_usage();
}