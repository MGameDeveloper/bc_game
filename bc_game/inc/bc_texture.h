#pragma once

#include "bc_ints.h"

struct bc_texture
{
	const char *path        = NULL;
	u32         id          = 0;
	i32         bound_idx   = 0;
	i32         use_count   = 0;
	i32         width       = 0;
	i32         height      = 0;
	float       width_norm  = 0.0f;
	float       height_norm = 0.0f;

	bc_texture();
	~bc_texture();
	bc_texture(const bc_texture&);
	bc_texture(const bc_texture&&);
	bc_texture& operator=(const bc_texture&);
	bc_texture& operator=(const bc_texture&&);
};