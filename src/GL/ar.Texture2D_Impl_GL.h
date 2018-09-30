
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

class Texture2D_Impl_GL
	: public Texture2D
{
private:
	Manager*					manager = nullptr;
	GLuint						texture = 0;
	std::vector<uint8_t>		resource;
	
public:
	Texture2D_Impl_GL();

	virtual ~Texture2D_Impl_GL();

	bool Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format, void* data, bool isEditable) override;

	bool Initialize(Manager* manager, const void* src, int32_t src_size, bool isEditable, bool isSRGB) override;

	bool Lock(TextureLockInfomation* info) override;
	
	void Unlock() override;

	virtual bool Save(std::vector<Color>& dst, int32_t& width, int32_t& height) override;

	GLuint GetTexture() const { return texture; }

	virtual std::array<uint64_t, 2> GetInternalObjects() const override { return std::array<uint64_t, 2> { (uint64_t)texture, 0 }; }
};

}
