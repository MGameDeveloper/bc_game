#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string.h>
#include <glad/glad.h>
#include "../inc/bc_texture.h"
#include "../inc/bc_log.h"
#include "../inc/bc_shader.h"

namespace pool
{
	struct bc_texture_data
	{
		const char* path        = NULL;
		u32         id          = 0;
		i32         bound_idx   = 0;
		i32         use_count   = 0;
		i32         width       = 0;
		i32         height      = 0;
		float       width_norm  = 0.0f;
		float       height_norm = 0.0f;
	};

	static bc_texture_data textures[32];
	static i32             idx              = 0;
	static i32             count            = 32;
	static bool            should_rebind    = false;
	static bool            has_dumb_texture = false;

	static void setup_dumb_texture()
	{
		bc_texture_data* texture = &textures[idx++];

		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		u8 data[]{ 255, 255, 255, 255 };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		texture->use_count  += 1;
		texture->path        = "bc_dumb_texture";
		texture->width       = 1;
		texture->height      = 1;
		texture->width_norm  = 1.f / float(1);
		texture->height_norm = 1.f / float(1);
		should_rebind        = true;

		has_dumb_texture = true;
	}

	static i32 load(const char* path)
	{
		if (!has_dumb_texture)
		{
			bc_log::error("[ bc_texture ]: you must call bc_texture::init to initialize it");
			return -1;
		}

		bc_texture_data* texture = NULL;

		for (i32 i = 0; i < idx; ++i)
		{
			if (strcmp(textures[i].path, path) == 0)
			{
				texture = &textures[i];
				texture->use_count++;
				return texture->bound_idx;
			}
		}

		if (idx < count)
		{
			texture            = &textures[idx++];
			texture->bound_idx = idx - 1;
		}
		else
		{
			for (i32 i = 0; i < count; ++i)
			{
				if (textures[i].use_count <= 0)
				{
					texture = &textures[i];
					break;
				}
			}
		}

		if (!texture)
			return -1;

		GLint fmt;
		i32 w, h, comp;
		u8* data = stbi_load(path, &w, &h, &comp, 0);
		if (!data)
		{
			stbi_image_free(data);
			bc_log::error("failed to load texture from [ %s ]", path);
			return -1;
		}
		else
		{
			if (comp == 3)
				fmt = GL_RGB;
			else if (comp == 4)
				fmt = GL_RGBA;
			else
			{
				stbi_image_free(data);
				bc_log::error("texture [ %s ] unknown format");
				return -1;
			}
		}

		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);

		texture->use_count  += 1;
		texture->path        = path;
		texture->width       = w;
		texture->height      = h;
		texture->width_norm  = 1.f / float(w);
		texture->height_norm = 1.f / float(h);
		should_rebind        = true;

		return texture->bound_idx;
	}

	static void unload(bc_texture* texture)
	{
		if (!texture)
			return;

		bc_texture_data* t = &textures[texture->idx_into_pool()];

		t->use_count--;
		if (t->use_count <= 0)
		{
			glDeleteTextures(1, &t->id);

			t->path        = NULL;
			t->width       = 0;
			t->height      = 0;
			t->width_norm  = 0.f;
			t->height_norm = 0.f;
		}
	}
};

bc_texture::bc_texture(const char* path)
{
	pool_idx = pool::load(path);
	if (pool_idx == -1)
		bc_log::error("[ bc_texture( %s ) ]: failed", path);
}

bc_texture::~bc_texture()
{
	pool::unload(this);
	pool_idx = -1;
}

glm::vec2 bc_texture::size()
{
	if (pool_idx == -1)
		return glm::vec2(0.f);

	return glm::vec2(pool::textures[pool_idx].width, pool::textures[pool_idx].height);
}

glm::vec2 bc_texture::size_norm()
{
	if (pool_idx == -1)
		return glm::vec2(0.f);

	return glm::vec2(pool::textures[pool_idx].width_norm, pool::textures[pool_idx].height_norm);
}

i32 bc_texture::idx()
{
	if (pool_idx == -1)
		return 0;

	return pool::textures[pool_idx].bound_idx;
}

i32 bc_texture::use_count()
{
	if (pool_idx == -1)
		return 0;

	return pool::textures[pool_idx].use_count;
}

const char* bc_texture::path()
{
	if (pool_idx == -1)
		return 0;

	return pool::textures[pool_idx].path;
}

i32 bc_texture::idx_into_pool()
{
	if (pool_idx == -1)
		return 0;

	return pool_idx;
}

void bc_texture::bind(bc_shader* shader)
{
	if (!shader)
		return;

	static char texture_element[256];

	if (pool::should_rebind)
	{
		for (i32 idx = 0; idx < pool::count; ++idx)
		{
			if (pool::textures[idx].use_count > 0)
			{
				glActiveTexture(GL_TEXTURE0 + idx);
				glBindTexture(GL_TEXTURE_2D, pool::textures[idx].id);

				memset(texture_element, 0, 256);
				sprintf_s(texture_element, "textures[%d]", idx);

				shader->set_int(texture_element, pool::textures[idx].bound_idx);

				bc_log::trace("[ bc_texture::bind ]: binding %s at %d", pool::textures[idx].path, idx);
				bc_log::trace("[ bc_texture::bind ]: %s = %d", texture_element, pool::textures[idx].id);
			}
		}

		pool::should_rebind = false;
	}
}

void bc_texture::init()
{
	pool::setup_dumb_texture();
}