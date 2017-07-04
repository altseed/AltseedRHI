
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{
	class Context_Impl_GL
		: public Context
	{
	private:
		Manager*									manager = nullptr;
		DrawParameter								previousDrawParam;

		std::array<GLuint, MaxTextureCount>			samplers;

		void CreateRenderStates(Manager* manager);
		void UpdateRenderStates(const DrawParameter& param, bool forced);
	public:
		Context_Impl_GL();

		virtual ~Context_Impl_GL();

		bool Initialize(Manager* manager);

		void Begin() override;

		void End() override;

		void Draw(const DrawParameter& param) override;
	};

}
