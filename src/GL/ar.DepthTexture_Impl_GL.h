
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

	class DepthTexture_Impl_GL
		: public RenderTexture2D
	{
	private:
		GLuint	buffer = 0;

	public:
		DepthTexture_Impl_GL();

		virtual ~DepthTexture_Impl_GL();

		bool Initialize(Manager* manager, int32_t width, int32_t height);

		GLuint GetBuffer() const { return buffer; }
	};

}
