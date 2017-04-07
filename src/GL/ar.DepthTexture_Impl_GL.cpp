#include "ar.DepthTexture_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

namespace ar
{
	DepthTexture_Impl_GL::DepthTexture_Impl_GL()
	{
	
	}

	DepthTexture_Impl_GL::~DepthTexture_Impl_GL()
	{
		glDeleteTextures(1, &buffer);
	}

	bool DepthTexture_Impl_GL::Initialize(Manager* manager, int32_t width, int32_t height)
	{
		// Genarate ID
		glGenTextures(1, &buffer);

		// Bind texture
		glBindTexture(GL_TEXTURE_2D, buffer);

		// Generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);

		// Set filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		this->width = width;
		this->height = height;

		return true;
	}
}
