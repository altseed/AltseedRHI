
#include "ar.RenderTexture2D_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"

namespace ar
{

	RenderTexture2D_Impl_DX11::RenderTexture2D_Impl_DX11()
	{

	}

	RenderTexture2D_Impl_DX11::~RenderTexture2D_Impl_DX11()
	{
		SafeRelease(texture);
		SafeRelease(textureSRV);
		SafeRelease(textureRTV);
	}


	bool RenderTexture2D_Impl_DX11::Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format)
	{
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = width;
		TexDesc.Height = height;
		TexDesc.MipLevels = 1;
		TexDesc.ArraySize = 1;
		if (format == TextureFormat::R8G8B8A8_UNORM)
		{
			TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		else if (format == TextureFormat::R16G16B16A16_FLOAT)
		{
			TexDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
		else if (format == TextureFormat::R32G32B32A32_FLOAT)
		{
			TexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (format == TextureFormat::R8G8B8A8_UNORM_SRGB)
		{
			TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		}
		else if (format == TextureFormat::R16G16_FLOAT)
		{
			TexDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
		}
		else if (format == TextureFormat::R8_UNORM)
		{
			TexDesc.Format = DXGI_FORMAT_R8_UNORM;
		}
		else
		{
			return nullptr;
		}

		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = 0;


		hr = m->GetDevice()->CreateTexture2D(&TexDesc, nullptr, &texture);
		if (FAILED(hr))
		{
			goto End;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = TexDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = TexDesc.MipLevels;

		hr = m->GetDevice()->CreateShaderResourceView(texture, &desc, &textureSRV);
		if (FAILED(hr))
		{
			goto End;
		}

		hr = m->GetDevice()->CreateRenderTargetView(texture, NULL, &textureRTV);
		if (FAILED(hr))
		{
			goto End;
		}

		this->manager = manager;
		this->width = width;
		this->height = height;
		this->format = format;

		return true;

	End:;
		SafeRelease(texture);
		SafeRelease(textureSRV);
		SafeRelease(textureRTV);
		return false;
	}


	bool RenderTexture2D_Impl_DX11::Save(std::vector<Color>& dst, int32_t& width, int32_t& height)
	{
		auto m = (Manager_Impl_DX11*)manager;
		width = this->width;
		height = this->height;

		return m->SaveTexture(dst, texture, this->width, this->height);
	}
}