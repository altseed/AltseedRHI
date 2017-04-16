
#pragma once

#if defined(_WIN32)
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#if defined(__APPLE__)
#include <GL/gl3.h>
#endif

#include <vector>

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

#include "../ar.BaseInternal.h"

namespace ar
{

}