#pragma once

#include "bc_ints.h"

struct bc_shader
{
	u32 id = 0;

	bc_shader()                             = default;
	bc_shader(const bc_shader&)             = delete;
	bc_shader(const bc_shader&&)            = delete;
	bc_shader& operator=(const bc_shader&)  = delete;
	bc_shader& operator=(const bc_shader&&) = delete;
	
	bc_shader(const char* vrtx_path, const char* frag_path);
	~bc_shader();

	void create(const char* vrtx_path, const char* frag_path);
	void destroy();

	void set_bool  (const char* var_name, bool value);
	void set_int   (const char* var_name, int value);
	void set_float (const char* var_name, float value);
	void set_4float(const char* var_name, float* values);
	void set_mat4  (const char* var_name, float* mat4);
	i32  get_uniform_location(const char* uniform_name);

	void bind();
};