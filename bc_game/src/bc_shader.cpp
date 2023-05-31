#include <string.h> // for memset
#include <glad/glad.h>
#include "../inc/bc_shader.h"
#include "../inc/bc_io.h"
#include "../inc/bc_log.h"


#define LOG_BUFFER_SIZE 512
static char log_buffer[LOG_BUFFER_SIZE];

static bool local_check_shader_compile_state(u32 id)
{
	memset(log_buffer, 0, LOG_BUFFER_SIZE);

	i32 success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, LOG_BUFFER_SIZE, NULL, log_buffer);
		bc_log::error("[ SHADER_COMPILATION ]: %s", log_buffer);
		return false;
	}

	return true;
}
static u32 local_create_shader(file_s* file, GLenum type)
{
	u32 id = glCreateShader(type);
	glShaderSource(id, 1, &file->data, NULL);

	glCompileShader(id);
	if (!local_check_shader_compile_state(id))
	{
		glDeleteShader(id);
		id = 0;
	}

	return id;
}

static bool local_check_shader_link_state(u32 id)
{
	memset(log_buffer, 0, 512);

	i32 success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, LOG_BUFFER_SIZE, NULL, log_buffer);
		bc_log::error("[ SHADER_LINKAGE ]: %s", log_buffer);
		return false;
	}

	return true;
}
static u32 local_create_shader_program(u32 vrtx_shader, u32 frag_shader)
{
	u32 id = glCreateProgram();
	glAttachShader(id, vrtx_shader);
	glAttachShader(id, frag_shader);

	glLinkProgram(id);
	if (!local_check_shader_link_state(id))
	{
		glDeleteProgram(id);
		id = 0;
	}

	return id;
}


bc_shader::bc_shader(const char* vrtx_path, const char* frag_path)
{
	create(vrtx_path, frag_path);
}

bc_shader::~bc_shader()
{
	destroy();
}

void bc_shader::create(const char* vrtx_path, const char* frag_path)
{
	file_s vrtx_file = io_loadfile(vrtx_path);
	file_s frag_file = io_loadfile(frag_path);
	if (io_isfile_valid(&vrtx_file) && io_isfile_valid(&frag_file))
	{
		u32 vrtx_shader = local_create_shader(&vrtx_file, GL_VERTEX_SHADER);
		u32 frag_shader = local_create_shader(&frag_file, GL_FRAGMENT_SHADER);
		if (vrtx_shader && frag_shader)
		{
			id = local_create_shader_program(vrtx_shader, frag_shader);

			// we can delete theme since they are linked 
			// we don't need them any more 
			glDeleteShader(vrtx_shader);
			glDeleteShader(frag_shader);
		}
	}
	io_unloadfile(&vrtx_file);
	io_unloadfile(&frag_file);

}

void bc_shader::destroy()
{
	glDeleteProgram(id);
}

void bc_shader::set_bool(const char* var_name, bool value)
{
	glUniform1i(get_uniform_location(var_name), (int)value);
}

void bc_shader::set_int(const char* var_name, int value)
{
	glUniform1i(get_uniform_location(var_name), value);
}

void bc_shader::set_float(const char* var_name, float value)
{
	glUniform1f(get_uniform_location(var_name), value);
}

void bc_shader::set_4float(const char* var_name, float* values)
{
	glUniform4fv(get_uniform_location(var_name), 1, values);
}

void bc_shader::set_mat4(const char* var_name, float* mat4)
{
	glUniformMatrix4fv(get_uniform_location(var_name), 1, GL_FALSE, mat4);
}

i32  bc_shader::get_uniform_location(const char* uniform_name)
{
	return glGetUniformLocation(id, uniform_name);
}

void bc_shader::bind()
{
	glUseProgram(id);
}