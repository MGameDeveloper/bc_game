#include "../inc/bc_clut.h"
#include "../inc/bc_shader.h"
#include "../inc/bc_mem_tracker.h"

static bc_memory_tracker_s bc_mem("[ BC_CLUT ]");

bc_clut::bc_clut(i32 clut_count, i32 color_per_clut)
{
	create(clut_count, color_per_clut);
}

bc_clut::~bc_clut()
{
	destroy();
}

void bc_clut::create(i32 clut_count, i32 color_per_clut)
{
	if (clut_count <= 0 || color_per_clut <= 0)
	{
		bc_log::warning("[ bc_clut::create( %d, %d) ]: one of the params is not correct", clut_count, color_per_clut);
		return;
	}

	u32 size   = (sizeof(glm::vec4) * (color_per_clut * clut_count));
	    colors = (glm::vec4*)bc_mem.alloc(size);
	if (colors)
	{
		bc_mem.zero(colors, size);
		this->clut_count = clut_count;
		this->color_count_per_clut = color_per_clut;
	}
	else
	{
		bc_log::error("[ bc_clut::create( %d, %d ) ]: creation failed", clut_count, color_per_clut);
	}
}

void bc_clut::destroy()
{
	if (colors)
	{
		bc_mem.dealloc(colors);
		clut_count           = 0;
		color_count_per_clut = 0;
	}
}

void bc_clut::set_color(i32 clut_idx, i32 color_idx, const glm::vec4& color)
{
	if (clut_count <= clut_idx || color_count_per_clut <= color_idx || !colors)
		return;

	static float f = 1.f / 255.f;
	colors[clut_idx * color_count_per_clut + color_idx] = (color * f);
}

glm::vec4 bc_clut::get_color(i32 clut_idx, i32 color_idx)
{
	static glm::vec4 dump_color(0.5, 0.5, 0.5, 1.f);

	if (clut_count <= clut_idx || color_count_per_clut <= color_idx || !colors)
		return dump_color;

	return colors[clut_idx * color_count_per_clut + color_idx];
}

void bc_clut::bind(bc_shader* shader)
{
	if (!shader || !colors)
		return;

	std::string clut_name(clut_array_name);
	for (u32 clut_idx = 0; clut_idx < clut->clut_count; ++clut_idx)
	{
		for (u32 color_idx = 0; color_idx < clut->color_count_per_clut; ++color_idx)
		{
			std::string var(clut_name + "[" + std::to_string(clut_idx) + "][" + std::to_string(color_idx) + "]");
	
			bc_shader::set_4float(shader, var.c_str(), &clut->colors[clut_idx * clut->color_count_per_clut + color_idx][0]);
		}
	}
}

void bc_clut::show_memory_consumption()
{
	bc_mem.print_usage();
}