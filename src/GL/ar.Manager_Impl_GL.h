
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{
	class Manager_Impl_GL
		: public Manager
	{
	private:
		GLuint			frameBuffer = 0;

		void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

	public:
		Manager_Impl_GL();
		virtual ~Manager_Impl_GL();

		ErrorCode Initialize(const ManagerInitializationParameter& param) override;

		void BeginScene(const SceneParameter& param) override;

		void EndScene() override;

		void Present() override;

		bool SaveScreen(std::vector<Color>& dst, int32_t& width, int32_t& height) override;

		bool SaveTexture(std::vector<Color>& bufs, GLuint texture, int32_t width, int32_t height);
	};
}
