
#include "ar.RenderTexture2D_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

namespace ar
{

	RenderTexture2D_Impl_GL::RenderTexture2D_Impl_GL()
	{

	}

	RenderTexture2D_Impl_GL::~RenderTexture2D_Impl_GL()
	{
		glDeleteTextures(1, &texture);
	}


	bool RenderTexture2D_Impl_GL::Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format)
	{
		glGenTextures(1, &texture);

		if (glGetError() != GL_NO_ERROR) return false;

		glBindTexture(GL_TEXTURE_2D, texture);

		if (format == TextureFormat::R8G8B8A8_UNORM)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				nullptr);
		}
		else if (format == TextureFormat::R16G16B16A16_FLOAT)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA16F,
				width,
				height,
				0,
				GL_RGBA,
				GL_FLOAT,
				nullptr);
		}
		else if (format == TextureFormat::R32G32B32A32_FLOAT)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA32F,
				width,
				height,
				0,
				GL_RGBA,
				GL_FLOAT,
				nullptr);
		}
		else if (format == TextureFormat::R8G8B8A8_UNORM_SRGB)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_SRGB8_ALPHA8,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				nullptr);
		}
		else if (format == TextureFormat::R16G16_FLOAT)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RG16F,
				width,
				height,
				0,
				GL_RG,
				GL_FLOAT,
				nullptr);
		}
		else if (format == TextureFormat::R8_UNORM)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_R8,
				width,
				height,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				nullptr);
		}
		else
		{
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		if (glGetError() != GL_NO_ERROR)
		{
			glDeleteTextures(1, &texture);
			return false;
		}

		GLCheckError();

		this->manager = manager;
		this->width = width;
		this->height = height;
		this->format = format;

		return true;
	}

	bool RenderTexture2D_Impl_GL::Save(std::vector<Color>& dst, int32_t& width, int32_t& height)
	{
		auto m = (Manager_Impl_GL*)manager;
		width = this->width;
		height = this->height;

		return m->SaveTexture(dst, texture, this->width, this->height);
	}
}
