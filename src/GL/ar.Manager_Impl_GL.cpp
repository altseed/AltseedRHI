
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

		ImageHelper::Terminate();
	}

	ErrorCode Manager_Impl_GL::Initialize(const ManagerInitializationParameter& param)
	{
#ifndef __APPLE__
		if (glewInit() != GLEW_OK)
		{
			return ErrorCode::FailedToInitializeGlew;
		}
#endif
		// Create frame buffer
		glGenFramebuffers(1, &frameBuffer);
		GLCheckError();

		// for frame buffer bugs
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GLCheckError();


		windowWidth = param.WindowWidth;
		windowHeight = param.WindowHeight;

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
}
