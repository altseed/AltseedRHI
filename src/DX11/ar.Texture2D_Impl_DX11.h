
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

class Texture2D_Impl_DX11
	: public Texture2D
{
private:
	Manager*					manager = nullptr;
	ID3D11Texture2D*			texture = nullptr;
	ID3D11ShaderResourceView*	textureSRV = nullptr;

	std::vector<uint8_t>		resource;

public:
	Texture2D_Impl_DX11();

	virtual ~Texture2D_Impl_DX11();

	bool Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format, void* data, bool isEditable) override;

	bool Initialize(Manager* manager, const void* src, int32_t src_size, bool isEditable, bool isSRGB) override;

	bool Lock(TextureLockInfomation* info) override;

	void Unlock() override;

	virtual bool Save(std::vector<Color>& dst, int32_t& width, int32_t& height) override;

	ID3D11ShaderResourceView* GetShaderResourceView() const { return textureSRV; }

	virtual std::array<uint64_t, 2> GetInternalObjects() const override { return std::array<uint64_t, 2> { (uint64_t)texture, (uint64_t)textureSRV}; }
};

}