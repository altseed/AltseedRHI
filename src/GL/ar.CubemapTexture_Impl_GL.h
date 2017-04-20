
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{
	class CubemapTexture_Impl_GL :
		public CubemapTexture
	{
	private:
		GLuint	texture = 0;

	public:

		CubemapTexture_Impl_GL();
		virtual ~CubemapTexture_Impl_GL();

		bool Initialize(Manager* manager, void* data, int32_t size) override;

		GLuint GetTexture() const { return texture; }
	};
}