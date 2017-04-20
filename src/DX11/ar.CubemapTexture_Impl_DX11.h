
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{
	class CubemapTexture_Impl_DX11 :
		public CubemapTexture
	{
	private:
		ID3D11Resource*	m_texture;
		ID3D11ShaderResourceView*	m_textureSRV;

	public:

		CubemapTexture_Impl_DX11();
		virtual ~CubemapTexture_Impl_DX11();

		bool Initialize(Manager* manager, void* data, int32_t size) override;

		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_textureSRV; }
	};
}