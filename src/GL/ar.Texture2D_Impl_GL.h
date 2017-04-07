
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

class Texture2D_Impl_GL
	: public Texture2D
{
private:
	GLuint						texture = 0;
	std::vector<uint8_t>		resource;
public:
	Texture2D_Impl_GL();

	virtual ~Texture2D_Impl_GL();

	bool Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format, void* data, bool isEditable);

	GLuint GetTexture() const { return texture; }
};

}
