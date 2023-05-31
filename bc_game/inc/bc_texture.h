#pragma once

#include <glm/glm.hpp>
#include "bc_ints.h"

class bc_texture
{
private:
	i32 pool_idx = 0;

public:
	bc_texture(const char* path);
	~bc_texture();
	
	bc_texture()                              = default;
	bc_texture(const bc_texture&)             = delete;
	bc_texture(const bc_texture&&)            = delete;
	bc_texture& operator=(const bc_texture&)  = delete;
	bc_texture& operator=(const bc_texture&&) = delete;

	glm::vec2   size();
	glm::vec2   size_norm();
	i32         idx();
	i32         use_count();
	const char* path();
	i32         idx_into_pool();
};