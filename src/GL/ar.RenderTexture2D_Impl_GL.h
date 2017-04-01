
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

class RenderTexture2D_Impl_GL
	: public RenderTexture2D
{
private:
	//ID3D11Texture2D*			texture = nullptr;
	//ID3D11ShaderResourceView*	textureSRV = nullptr;
	//ID3D11RenderTargetView*		textureRTV = nullptr;

public:
	RenderTexture2D_Impl_GL();

	virtual ~RenderTexture2D_Impl_GL();

	bool Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format);
};

}
