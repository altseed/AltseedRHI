
#include "ar.Texture2D_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

#include "../ar.ImageHelper.h"

namespace ar
{

	Texture2D_Impl_GL::Texture2D_Impl_GL()
	{

	}

	Texture2D_Impl_GL::~Texture2D_Impl_GL()
	{
		glDeleteTextures(1, &texture);
	}

	bool Texture2D_Impl_GL::Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format, void* data, bool isEditable)
	{
		glGenTextures(1, &texture);

		if (glGetError() != GL_NO_ERROR)
		{
			glDeleteTextures(1, &texture);
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, texture);

		int32_t intrenalFormat_;
		int32_t format_ = GL_RGBA;
		int32_t type;
		if (format == TextureFormat::R8G8B8A8_UNORM)
		{
			intrenalFormat_ = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
		}
		else if (format == TextureFormat::R16G16B16A16_FLOAT)
		{
			intrenalFormat_ = GL_RGBA16F;
			format_ = GL_RGBA;
			type = GL_HALF_FLOAT;
		}
		else if (format == TextureFormat::R32G32B32A32_FLOAT)
		{
			intrenalFormat_ = GL_RGBA32F;
			type = GL_FLOAT;
		}
		else if (format == TextureFormat::R8G8B8A8_UNORM_SRGB)
		{
			intrenalFormat_ = GL_SRGB8_ALPHA8;
			type = GL_UNSIGNED_BYTE;
		}
		else if (format == TextureFormat::R16G16_FLOAT)
		{
			intrenalFormat_ = GL_RG16F;
			format_ = GL_RG;
			type = GL_HALF_FLOAT;
		}
		else if (format == TextureFormat::R8_UNORM)
		{
#ifdef __APPLE__
			intrenalFormat_ = GL_RED;
			format_ = GL_RED;
#else
			intrenalFormat_ = GL_LUMINANCE;
			format_ = GL_LUMINANCE;
#endif
			type = GL_UNSIGNED_BYTE;
		}
		else
		{
			return false;
		}

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			intrenalFormat_,
			width,
			height,
			0,
			format_,
			type,
			data);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (glGetError() != GL_NO_ERROR)
		{
			glDeleteTextures(1, &texture);
			return nullptr;
		}

		if (isEditable)
		{
			resource.resize(width * height *  ImageHelper::GetPitch(format));

			if (data != nullptr)
			{
				memcpy(resource.data(), data, resource.size());
			}
			else
			{
				memset(resource.data(), 0, resource.size());
			}
		}

		this->width = width;
		this->height = height;

		GLCheckError();
		return false;
	}
}
