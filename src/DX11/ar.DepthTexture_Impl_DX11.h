
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

	class DepthTexture_Impl_DX11
		: public DepthTexture
	{
	private:
		ID3D11Texture2D*			depthBuffer = nullptr;
		ID3D11DepthStencilView*		depthStencilView = nullptr;
		ID3D11ShaderResourceView*	depthSRV = nullptr;


	public:
		DepthTexture_Impl_DX11();

		virtual ~DepthTexture_Impl_DX11();

		bool Initialize(Manager* manager, int32_t width, int32_t height) override;

		ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView; }

		ID3D11ShaderResourceView* GetShaderResourceView() const { return depthSRV; }
	};

}