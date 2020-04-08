
#include "ar.Manager_Impl_GL.h"

#include "ar.RenderTexture2D_Impl_GL.h"
#include "ar.DepthTexture_Impl_GL.h"

#include "../ar.ImageHelper.h"

namespace ar
{
	void Manager_Impl_GL::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
	{
		glViewport(x, y, width, height);
	}

	Manager_Impl_GL::Manager_Impl_GL()
	{
		ImageHelper::Initizlize();
		deviceType = GraphicsDeviceType::OpenGL;
	}

	Manager_Impl_GL::~Manager_Impl_GL()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &frameBuffer);

#ifdef __APPLE__
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vao);
		GLCheckError();
#endif

		ImageHelper::Terminate();
	}

	ErrorCode Manager_Impl_GL::Initialize(const ManagerInitializationParameter& param)
	{
#ifdef _WIN32
		if (glewInit() != GLEW_OK)
		{
			return ErrorCode::FailedToInitializeGlew;
		}
#else
		// Fixed version(TODO : Change)
		version = 33;
#endif

		if (param.ColorSpace == ColorSpaceType::LinearSpace)
		{
			glEnable(GL_FRAMEBUFFER_SRGB);
		}

		// Create frame buffer
		glGenFramebuffers(1, &frameBuffer);
		GLCheckError();

		// for frame buffer bugs
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GLCheckError();

#ifdef __APPLE__
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		GLCheckError();
#endif

		windowWidth = param.WindowWidth;
		windowHeight = param.WindowHeight;
		colorSpaceType = param.ColorSpace;

#ifdef _WIN32
		version = 21;
		if (GLEW_VERSION_3_1) version = 31;
		if (GLEW_VERSION_3_2) version = 32;
		if (GLEW_VERSION_3_3) version = 33;
		if (GLEW_VERSION_4_0) version = 40;
#endif

		return ErrorCode::OK;
	}

	void Manager_Impl_GL::BeginScene(const SceneParameter& param)
	{
		GLCheckError();

		// reset
		for (int32_t i = 0; i < MaxTextureCount; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glActiveTexture(GL_TEXTURE0);
		GLCheckError();

		if (param.RenderTargets[0] == nullptr)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDrawBuffer(GL_BACK);
			SetViewport(0, 0, windowWidth, windowHeight);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

			GLuint cb[] = { 0, 0, 0, 0 };
			GLuint db = 0;

			for (int32_t i = 0; i < MaxRenderTarget; i++)
			{
				if (param.RenderTargets[i] == nullptr) continue;
				cb[i] = ((RenderTexture2D_Impl_GL*)param.RenderTargets[i])->GetTexture();
			}

			if (param.DepthTarget != nullptr)
			{
				db = ((DepthTexture_Impl_GL*)param.DepthTarget)->GetTexture();
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb[0], 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, cb[1], 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, cb[2], 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, cb[3], 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, db, 0);

			static const GLenum bufs[] = {
				GL_COLOR_ATTACHMENT0,
				GL_COLOR_ATTACHMENT1,
				GL_COLOR_ATTACHMENT2,
				GL_COLOR_ATTACHMENT3,
			};
			glDrawBuffers(4, bufs);

			GLCheckError();

			SetViewport(0, 0, param.RenderTargets[0]->GetWidth(), param.RenderTargets[0]->GetHeight());
		}

		GLCheckError();
	}

	void Manager_Impl_GL::EndScene()
	{

	}

	void Manager_Impl_GL::Present()
	{
    }

	void Manager_Impl_GL::Clear(bool isColorTarget, bool isDepthTarget, const Color& color)
	{
		GLbitfield bit = 0;
		if (isColorTarget)
		{
			bit = bit | GL_COLOR_BUFFER_BIT;
			glClearColor(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
		}

		if (isDepthTarget)
		{
			// GL_DEPTH_TEST & WRITE を有効化しないとクリアできない
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);

			bit = bit | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
			glClearDepth(1.0f);
		}

		if (bit != 0)
		{
			glClear(bit);
		}

		GLCheckError();
	}

	bool Manager_Impl_GL::SaveScreen(std::vector<Color>& dst, int32_t& width, int32_t& height)
	{
		GLCheckError();

		glFlush();
		glFinish();

		glViewport(0, 0, windowWidth, windowHeight);
		glReadBuffer(GL_BACK);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		width = windowWidth;
		height = windowHeight;

		std::vector<Color> temp;
		temp.resize(width * height);
		dst.resize(width * height);

		glReadPixels(
			0,
			0,
			width,
			height,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			(void*)temp.data());

		FlipYInternal(dst, temp, width, height);

		GLCheckError();

		return true;
	}

	bool Manager_Impl_GL::SetWindowSize(int32_t width, int32_t height)
	{
		windowWidth = width;
		windowHeight = height;
		return true;
	}

	void Manager_Impl_GL::ApplyVAO()
	{
#ifdef __APPLE__
		glBindVertexArray(vao);
		GLCheckError();
#endif
	}

	bool Manager_Impl_GL::SaveTexture(std::vector<Color>& dst, GLuint texture, int32_t width, int32_t height)
	{
		GLCheckError();

		std::vector<Color> temp;
		temp.resize(width * height);
		dst.resize(width * height);

		glBindTexture(GL_TEXTURE_2D, texture);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp.data());

		glBindTexture(GL_TEXTURE_2D, 0);

		FlipYInternal(dst, temp, width, height);

		GLCheckError();

		return true;
	}
}
