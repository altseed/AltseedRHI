
#include "ar.Texture2D_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

#include "../ar.ImageHelper.h"

#include "../3rdParty/nv_dds/nv_dds.h"

#include <sstream>
#include <iostream>

namespace ar
{
	struct LoadTextureInfo
	{
		ar::Manager* manager = nullptr;
		ar::Texture2D* texture = nullptr;
		bool isEditable = false;
		bool isSRGB = false;
	};

	static void loadTexture(const uint8_t* data, int32_t width, int32_t height, void* userData)
	{
		auto p = (LoadTextureInfo*)userData;

		if (p->isSRGB)
		{
			if (!p->texture->Initialize(p->manager, width, height, ar::TextureFormat::R8G8B8A8_UNORM_SRGB, (void*)data, p->isEditable))
			{
				p->texture = nullptr;
			}
		}
		else
		{
			if (!p->texture->Initialize(p->manager, width, height, ar::TextureFormat::R8G8B8A8_UNORM, (void*)data, p->isEditable))
			{
				p->texture = nullptr;
			}
		}
	}

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

		if (data != nullptr)
		{
			std::vector<uint8_t> resource_rev;

			FlipYInternal(resource_rev, (const uint8_t*)data, width, height, format);

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				intrenalFormat_,
				width,
				height,
				0,
				format_,
				type,
				resource_rev.data());
		}
		else
		{

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				intrenalFormat_,
				width,
				height,
				0,
				format_,
				type,
				nullptr);
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);

		if (glGetError() != GL_NO_ERROR)
		{
			glDeleteTextures(1, &texture);
			return false;
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
		this->format = format;
		this->manager = manager;

		GLCheckError();
		return true;
	}

	bool Texture2D_Impl_GL::Initialize(Manager* manager, const void* src, int32_t src_size, bool isEditable, bool isSRGB)
	{
		if (src_size == 0) return false;

		if (ImageHelper::IsPNG(src, src_size))
		{
			LoadTextureInfo ltInfo;
			ltInfo.manager = manager;
			ltInfo.texture = this;
			ltInfo.isEditable = isEditable;
			ltInfo.isSRGB = isSRGB;

			if (ar::ImageHelper::LoadPNG(loadTexture, &ltInfo, src, src_size) &&
				ltInfo.texture != nullptr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (ImageHelper::IsDDS(src, src_size))
		{
			std::vector<char> d;
			d.resize(src_size);
			memcpy(d.data(), src, src_size);
			nv_dds::CDDSImage image;
			IMemoryStream stream(d.data(), d.size());
			image.load(stream);

			if (image.get_format() == GL_RGBA)
			{
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);

				glTexImage2D(GL_TEXTURE_2D, 0, image.get_components(), image.get_width(),
					image.get_height(), 0, image.get_format(), GL_UNSIGNED_BYTE, image);

				for (int i = 0; i < image.get_num_mipmaps(); i++)
				{
					glTexImage2D(GL_TEXTURE_2D, i + 1, image.get_components(),
						image.get_mipmap(i).get_width(), image.get_mipmap(i).get_height(),
						0, image.get_format(), GL_UNSIGNED_BYTE, image.get_mipmap(i));
				}

				glBindTexture(GL_TEXTURE_2D, 0);

				this->width = image.get_width();
				this->height = image.get_height();
				this->format = TextureFormat::R8G8B8A8_UNORM;
				this->manager = manager;

				return true;
			}

			if (image.get_format() == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ||
				image.get_format() == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ||
				image.get_format() == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
			{
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);

				glCompressedTexImage2D(GL_TEXTURE_2D, 0, image.get_format(),
					image.get_width(), image.get_height(), 0, image.get_size(),
					image);

				for (int i = 0; i < image.get_num_mipmaps(); i++)
				{
					auto mipmap = image.get_mipmap(i);

					glCompressedTexImage2D(GL_TEXTURE_2D, i + 1, image.get_format(),
						mipmap.get_width(), mipmap.get_height(), 0, mipmap.get_size(),
						mipmap);
				}

				glBindTexture(GL_TEXTURE_2D, 0);

				this->width = image.get_width();
				this->height = image.get_height();
				this->manager = manager;

				if (image.get_format() == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
				{
					this->format = TextureFormat::BC1;
				}
				else if (image.get_format() == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
				{
					this->format = TextureFormat::BC2;
				}
				else if (image.get_format() == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
				{
					this->format = TextureFormat::BC3;
				}
				else
				{
					glDeleteTextures(1, &texture);
					texture = 0;
					return false;
				}

				return true;
			}

		}

		return false;
	}

	bool Texture2D_Impl_GL::Lock(TextureLockInfomation* info)
	{
		if (info == nullptr) return false;
		if (resource.size() == 0) return false;

		info->Pixels = &(resource[0]);
		info->Pitch = ImageHelper::GetPitch(format);
		info->Width = width;
		info->Height = height;
		return true;
	}

	void Texture2D_Impl_GL::Unlock()
	{
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
		std::vector<uint8_t> resource_rev;
		FlipYInternal(resource_rev, (const uint8_t*)resource.data(), width, height, format);

		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0,
			0,
			width,
			height,
			format_,
			type,
			resource_rev.data());

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool Texture2D_Impl_GL::Save(std::vector<Color>& dst, int32_t& width, int32_t& height)
	{
		auto m = (Manager_Impl_GL*)manager;
		width = this->width;
		height = this->height;

		return m->SaveTexture(dst, texture, this->width, this->height);
	}
}
