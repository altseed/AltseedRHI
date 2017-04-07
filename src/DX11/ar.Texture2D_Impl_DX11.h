
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

class Texture2D_Impl_DX11
	: public RenderTexture2D
{
private:
	ID3D11Texture2D*			texture = nullptr;
	ID3D11ShaderResourceView*	textureSRV = nullptr;
	ID3D11RenderTargetView*		textureRTV = nullptr;

	std::vector<uint8_t>		resource;

public:
	Texture2D_Impl_DX11();

	virtual ~Texture2D_Impl_DX11();

	bool Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format, void* data, bool isEditable);

	ID3D11ShaderResourceView* GetShaderResourceView() const { return textureSRV; }
};

}