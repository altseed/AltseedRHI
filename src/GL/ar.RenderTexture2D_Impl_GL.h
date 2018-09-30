
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

class RenderTexture2D_Impl_GL
	: public RenderTexture2D
{
private:
	Manager*		manager = nullptr;
	GLuint	texture = 0;

public:
	RenderTexture2D_Impl_GL();

	virtual ~RenderTexture2D_Impl_GL();

	bool Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format) override;

	virtual bool Save(std::vector<Color>& dst, int32_t& width, int32_t& height) override;

	GLuint GetTexture() const { return texture; }

	virtual std::array<uint64_t, 2> GetInternalObjects() const override { return std::array<uint64_t, 2> { (uint64_t)texture, 0 }; }

};

}
