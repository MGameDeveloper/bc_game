#pragma once

#include "bc_ints.h"
#include <glm/glm.hpp>

struct bc_shader;

struct bc_clut
{
	glm::vec4 *colors;
	i32        clut_count;
	i32        color_count_per_clut;

	bc_clut()                           = default;
	bc_clut(const bc_clut&)             = delete;
	bc_clut(const bc_clut&&)            = delete;
	bc_clut& operator=(const bc_clut&)  = delete;
	bc_clut& operator=(const bc_clut&&) = delete;

	bc_clut(i32 clut_count, i32 color_per_clut);
	~bc_clut();
	
	void      create(i32 clut_count, i32 color_per_clut);
	void      destroy();
	void      set_color(i32 clut_idx, i32 color_idx, const glm::vec4& color);
	glm::vec4 get_color(i32 clut_idx, i32 color_idx);

	void bind(bc_shader* shader);

	static void show_memory_consumption();
};