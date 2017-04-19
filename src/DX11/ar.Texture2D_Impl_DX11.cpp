
#include "ar.Texture2D_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"

#include "../ar.ImageHelper.h"

#include "../3rdParty/DirectXToolKit/PlatformHelpers.h"
#include "../3rdParty/DirectXToolKit/DDSTextureLoader.h"

namespace ar
{
	struct LoadTextureInfo
	{
		ar::Manager* manager = nullptr;
		ar::Texture2D* texture = nullptr;
		bool isEditable = false;
		bool isSRGB = false;
	};

	static void loadTexture(const uint8_t* data, int32_t width, int32_t height, void* userData)
	{
		auto p = (LoadTextureInfo*)userData;

		if (p->isSRGB)
		{
			if (!p->texture->Initialize(p->manager, width, height, ar::TextureFormat::R8G8B8A8_UNORM_SRGB, (void*)data, p->isEditable))
			{
				p->texture = nullptr;
			}
		}
		else
		{
			if (!p->texture->Initialize(p->manager, width, height, ar::TextureFormat::R8G8B8A8_UNORM, (void*)data, p->isEditable))
			{
				p->texture = nullptr;
			}
		}
	}

	Texture2D_Impl_DX11::Texture2D_Impl_DX11()
	{

	}

	Texture2D_Impl_DX11::~Texture2D_Impl_DX11()
	{
		SafeRelease(texture);
		SafeRelease(textureSRV);
	}


	bool Texture2D_Impl_DX11::Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format, void* data, bool isEditable)
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
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		initData.SysMemPitch = width * ImageHelper::GetPitch(format);
		initData.SysMemSlicePitch = initData.SysMemPitch * height;

		hr = m->GetDevice()->CreateTexture2D(&TexDesc, data != nullptr ? &initData : nullptr, &texture);
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

		if (isEditable)
		{
			resource.resize(width * height *  ImageHelper::GetPitch(format));
			
			if (data != nullptr)
			{
				memcpy(resource.data(), data, resource.size());
			}
			else
			{
				memset(resource.data(), 0, resource.size());
			}
		}

		this->width = width;
		this->height = height;
		this->format = format;
		this->manager = manager;

		return true;

	End:;
		SafeRelease(texture);
		SafeRelease(textureSRV);
		return false;
	}

	bool Texture2D_Impl_DX11::Initialize(Manager* manager, const void* src, int32_t src_size, bool isEditable, bool isSRGB)
	{
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;

		if (src_size == 0) return nullptr;

		if (ImageHelper::IsPNG(src, src_size))
		{
			LoadTextureInfo ltInfo;
			ltInfo.manager = manager;
			ltInfo.texture = this;
			ltInfo.isEditable = isEditable;
			ltInfo.isSRGB = isSRGB;

			if (ar::ImageHelper::LoadPNG(loadTexture, &ltInfo, src, src_size) &&
				ltInfo.texture != nullptr)
			{
				return true;
			}
			else
			{
				goto End;
			}
		}
		if (ImageHelper::IsDDS(src, src_size))
		{
			ID3D11Resource* _texture = nullptr;
			ID3D11ShaderResourceView* _textureSRV = nullptr;

			hr = DirectX::CreateDDSTextureFromMemory(
				m->GetDevice(),
				(const uint8_t*)src,
				src_size,
				&_texture,
				&_textureSRV);

			if (_texture == nullptr || _textureSRV == nullptr)
			{
				SafeRelease(_texture);
				SafeRelease(_textureSRV);
				goto End;
			}

			textureSRV = _textureSRV;

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			textureSRV->GetDesc(&desc);

			texture = (ID3D11Texture2D*)_texture;
			D3D11_TEXTURE2D_DESC desc_;
			texture->GetDesc(&desc_);

			if (desc.ViewDimension != D3D_SRV_DIMENSION_TEXTURE2D)
			{
				goto End;
			}

			this->width = desc_.Width;
			this->height = desc_.Height;
			this->format = GetTextureFormat(desc.Format);
			this->manager = manager;
		}

		return true;

	End:;
		SafeRelease(texture);
		SafeRelease(textureSRV);
		return false;
	}

	bool Texture2D_Impl_DX11::Lock(TextureLockInfomation* info)
	{
		if (info == nullptr) return false;
		if (resource.size() == 0) return false;

		info->Pixels = resource.data();
		info->Pitch = ImageHelper::GetPitch(format);
		info->Width = width;
		info->Height = height;
		return true;
	}

	void Texture2D_Impl_DX11::Unlock()
	{
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;

		m->GetContext()->UpdateSubresource(
			texture,
			0,
			NULL,
			resource.data(),
			width * ImageHelper::GetPitch(format),
			0);
	}

	bool Texture2D_Impl_DX11::Save(std::vector<Color>& dst, int32_t& width, int32_t& height)
	{
		auto m = (Manager_Impl_DX11*)manager;
		width = this->width;
		height = this->height;

		return m->SaveTexture(dst, texture, this->width, this->height);
	}
}