
#include "ar.CubemapTexture_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"

#include "../ar.ImageHelper.h"

#include "../3rdParty/DirectXToolKit/PlatformHelpers.h"
#include "../3rdParty/DirectXToolKit/DDSTextureLoader.h"

namespace ar
{
	CubemapTexture_Impl_DX11::CubemapTexture_Impl_DX11()
	{
		m_texture = nullptr;
		m_textureSRV = nullptr;
	}

	CubemapTexture_Impl_DX11::~CubemapTexture_Impl_DX11()
	{
		SafeRelease(m_texture);
		SafeRelease(m_textureSRV);
	}

	bool CubemapTexture_Impl_DX11::Initialize(Manager* manager, void* data, int32_t size)
	{
		if (!ImageHelper::IsDDS(data, size)) return false;

		auto m = (Manager_Impl_DX11*)manager;

		auto hr = DirectX::CreateDDSTextureFromMemory(
			m->GetDevice(),
			(const uint8_t*)data,
			size,
			&m_texture,
			&m_textureSRV);

		if (FAILED(hr))
		{
			goto End;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC cubeDesc;

		m_textureSRV->GetDesc(&cubeDesc);

		if (cubeDesc.ViewDimension != D3D_SRV_DIMENSION_TEXTURECUBE)
		{
			goto End;
		}

		ID3D11Texture2D* texture_ = (ID3D11Texture2D*)m_texture;
		D3D11_TEXTURE2D_DESC desc;
		texture_->GetDesc(&desc);

		this->width = desc.Width;
		this->height = desc.Height;
		this->mipmapCount = cubeDesc.TextureCube.MipLevels;
		this->format = GetTextureFormat(desc.Format);

		return true;

	End:;

		SafeRelease(m_texture);
		SafeRelease(m_textureSRV);
		return false;
	}

}