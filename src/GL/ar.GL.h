
#pragma once

#if defined(_WIN32)

#define GLEW_STATIC
#include <GL/glew.h>

#elif defined(__APPLE__)

#include <OpenGL/gl3.h>

#else

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#endif

#include <streambuf>
#include <istream>
#include <string.h>

#include "../ar.BaseInternal.h"
#include "../ar.ImageHelper.h"

static std::vector<GLenum> GLCheckError_Impl(const char *file, const int line)
{
	std::vector<GLenum> codes;
	GLenum __code = glGetError();
	while (__code != GL_NO_ERROR) {
		codes.push_back(__code);
#ifdef _DEBUG
		printf("GLError filename = %s , line = %d, error = %d\n", file, line, __code);
#endif
		__code = glGetError();

		if (codes.size() > 128) break;
	}
	return codes;
}
#define GLCheckError() GLCheckError_Impl(__FILE__, __LINE__)



namespace ar
{
	struct MemoryStreamBuf 
		: public std::streambuf
	{
		MemoryStreamBuf(const void* data, size_t size)
		{
			auto p = (char*)(data);
			this->setg(p, p, p + size);
		}
	};
	struct IMemoryStream : 
		virtual public MemoryStreamBuf, std::istream
	{
		IMemoryStream(const void* data, size_t size)
			: MemoryStreamBuf(data, size)
			, std::istream(static_cast<std::streambuf*>(this)) 
		{
		}
	};

	static void FlipYInternal(std::vector<uint8_t> &dst, const uint8_t* src, int32_t width, int32_t height, TextureFormat format)
	{
		auto pitch = ImageHelper::GetPitch(format);
		auto size = width * height * pitch;
		dst.resize(size);

		for (auto y = 0; y < height; y++)
		{
			for (auto x = 0; x < width; x++)
			{
				for (auto p = 0; p < pitch; p++)
				{
					dst[p + (x + y * width) * pitch] = src[p + (x + (height - y - 1) * width) * pitch];
				}
			}
		}
	}

	static void FlipYInternal(std::vector<Color> &dst, std::vector<Color> src, int32_t width, int32_t height)
	{
		auto size = width * height;
		dst.resize(size);

		for (auto y = 0; y < height; y++)
		{
			for (auto x = 0; x < width; x++)
			{
				dst[x + y * width] = src[x + (height - y - 1) * width];
			}
		}
	}
}