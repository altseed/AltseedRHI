#include "ar.DepthTexture_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"

namespace ar
{
	DepthTexture_Impl_DX11::DepthTexture_Impl_DX11()
	{
	
	}

	DepthTexture_Impl_DX11::~DepthTexture_Impl_DX11()
	{
		SafeRelease(depthBuffer);
		SafeRelease(depthStencilView);
		SafeRelease(depthSRV);
	}

	bool DepthTexture_Impl_DX11::Initialize(Manager* manager, int32_t width, int32_t height)
	{
		auto m = (Manager_Impl_DX11*)manager;

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;


		if (FAILED(m->GetDevice()->CreateTexture2D(&desc, NULL, &depthBuffer)))
		{
			goto End;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
		viewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		viewDesc.Flags = 0;
		if (FAILED(m->GetDevice()->CreateDepthStencilView(depthBuffer, &viewDesc, &depthStencilView)))
		{
			goto End;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;

		if (FAILED(m->GetDevice()->CreateShaderResourceView(depthBuffer, &srvDesc, &depthSRV)))
		{
			goto End;
		}

		this->width = width;
		this->height = height;

		return true;

	End:;
		SafeRelease(depthBuffer);
		SafeRelease(depthStencilView);
		SafeRelease(depthSRV);
		return false;
	}
}