
#pragma once

#if defined(_WIN32)
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#if defined(__APPLE__)
#include <GL/gl3.h>
#endif
