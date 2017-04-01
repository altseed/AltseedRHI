
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

	class DepthTexture_Impl_GL
		: public RenderTexture2D
	{
	private:
		//ID3D11Texture2D*			depthBuffer = nullptr;
		//ID3D11DepthStencilView*		depthStencilView = nullptr;
		//ID3D11ShaderResourceView*	depthSRV = nullptr;


	public:
		DepthTexture_Impl_GL();

		virtual ~DepthTexture_Impl_GL();

		bool Initialize(Manager* manager, int32_t width, int32_t height);
	};

}
