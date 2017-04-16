
#include "ar.Manager_Impl_DX11.h"

#include "ar.RenderTexture2D_Impl_DX11.h"
#include "ar.DepthTexture_Impl_DX11.h"

#include "../ar.ImageHelper.h"

namespace ar
{
	void Manager_Impl_DX11::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
	{
		D3D11_VIEWPORT vp;
		vp.TopLeftX = x;
		vp.TopLeftY = y;
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		context->RSSetViewports(1, &vp);
	}

	Manager_Impl_DX11::Manager_Impl_DX11()
	{
		ImageHelper::Initizlize();
		deviceType = GraphicsDeviceType::DirectX11;
	}

	Manager_Impl_DX11::~Manager_Impl_DX11()
	{
		SafeRelease(device);
		SafeRelease(context);
		SafeRelease(dxgiFactory);
		SafeRelease(adapter);
		SafeRelease(swapChain);
		SafeRelease(defaultBack);
		SafeRelease(defaultBackRenderTargetView);

		ImageHelper::Terminate();
	}

	ErrorCode Manager_Impl_DX11::Initialize(const ManagerInitializationParameter& param)
	{
		HRESULT hr;
		ErrorCode result;
		std::vector<IDXGIAdapter1*>	adapters;

		// Create factory
		hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&dxgiFactory);
		if (dxgiFactory == NULL)
		{
			result = ErrorCode::FailedToCreateFactory;
			goto End;
		}

		// Get adapter
		for (int32_t i = 0;; i++)
		{
			IDXGIAdapter1* temp = 0;
			if (dxgiFactory->EnumAdapters1(i, &temp) != DXGI_ERROR_NOT_FOUND)
			{
				adapters.push_back(temp);
			}
			else
			{
				break;
			}
		}

		if (adapters.size() > 0)
		{
			SafeAddRef(adapters[0]);
			adapter = adapters[0];
		}

		if (adapter == nullptr)
		{
			result = ErrorCode::FailedToGetAdapter;
			goto End;
		}

		// Create device
		UINT debugFlag = 0;

#if _DEBUG
		debugFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL flevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};
		int32_t flevelCount = sizeof(flevels) / sizeof(D3D_FEATURE_LEVEL);

		D3D_FEATURE_LEVEL currentFeatureLevel;

		hr = D3D11CreateDevice(
			adapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			debugFlag,
			flevels,
			flevelCount,
			D3D11_SDK_VERSION,
			&device,
			&currentFeatureLevel,
			&context);

		if FAILED(hr)
		{
			result = ErrorCode::FailedToCreateDevice;
			goto End;
		}

		// Create swap chain
		DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
		hDXGISwapChainDesc.BufferDesc.Width = param.WindowWidth;
		hDXGISwapChainDesc.BufferDesc.Height = param.WindowHeight;
		hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		
		if (param.ColorSpace == ColorSpaceType::LinearSpace)
		{
			hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		}
		else
		{
			hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		hDXGISwapChainDesc.SampleDesc.Count = 1;
		hDXGISwapChainDesc.SampleDesc.Quality = 0;
		hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		hDXGISwapChainDesc.BufferCount = 1;
		hDXGISwapChainDesc.OutputWindow = (HWND)param.Handles[0];
		hDXGISwapChainDesc.Windowed = param.IsFullscreenMode ? FALSE : TRUE;
		hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		if (FAILED(dxgiFactory->CreateSwapChain(device, &hDXGISwapChainDesc, &swapChain)))
		{		
			result = ErrorCode::FailedToCreateSwapChain;
			goto End;
		}

		if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&defaultBack)))
		{
			result = ErrorCode::FailedToGetBackBuffer;
			goto End;
		}

		if (FAILED(device->CreateRenderTargetView(defaultBack, NULL, &defaultBackRenderTargetView)))
		{
			result = ErrorCode::FailedToCreateBackBufferTarget;
			goto End;
		}

		for (auto a : adapters)
		{
			a->Release();
		}

		windowWidth = param.WindowWidth;
		windowHeight = param.WindowHeight;

		result = ErrorCode::OK;
		return result;

	End:;
		for (auto a : adapters)
		{
			a->Release();
		}

		SafeRelease(device);
		SafeRelease(context);
		SafeRelease(dxgiFactory);
		SafeRelease(adapter);
		SafeRelease(swapChain);
		SafeRelease(defaultBack);
		SafeRelease(defaultBackRenderTargetView);

		return result;
	}

	void Manager_Impl_DX11::BeginScene(const SceneParameter& param)
	{
		// reset
		for (int32_t i = 0; i < MaxTextureCount; i++)
		{
			ID3D11ShaderResourceView* rv = { nullptr };
			GetContext()->VSSetShaderResources(i, 1, &rv);
			GetContext()->PSSetShaderResources(i, 1, &rv);
		}

		if (param.RenderTargets[0] == nullptr)
		{
			context->OMSetRenderTargets(1, &defaultBackRenderTargetView, nullptr);
			SetViewport(0, 0, windowWidth, windowHeight);
		}
		else
		{
			ID3D11RenderTargetView* rt[MaxRenderTarget] = { nullptr, nullptr, nullptr, nullptr };
			ID3D11DepthStencilView* ds = nullptr;

			for (int32_t i = 0; i < MaxRenderTarget; i++)
			{
				if (param.RenderTargets[i] == nullptr) continue;

				rt[i] = ((RenderTexture2D_Impl_DX11*)param.RenderTargets[i])->GetRenderTargetView();
			}

			if (param.DepthTarget != nullptr)
			{
				ds = ((DepthTexture_Impl_DX11*)param.DepthTarget)->GetDepthStencilView();
			}

			if (rt != nullptr)
			{
				context->OMSetRenderTargets(4, rt, ds);
				SetViewport(0, 0, param.RenderTargets[0]->GetWidth(), param.RenderTargets[0]->GetHeight());
			}
		}

	}

	void Manager_Impl_DX11::EndScene()
	{

	}

	void Manager_Impl_DX11::Present()
	{
		// 同期しない
		swapChain->Present(0, 0);
	}

	bool Manager_Impl_DX11::SaveTexture(std::vector<Color>& dst, ID3D11Resource* texture, int32_t width, int32_t height)
	{
		ID3D11Texture2D* texture_ = nullptr;

		HRESULT hr;

		D3D11_TEXTURE2D_DESC desc;
		desc.ArraySize = 1;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_STAGING;

		hr = GetDevice()->CreateTexture2D(&desc, 0, &texture_);
		if (FAILED(hr))
		{
			goto END;
		}

		GetContext()->CopyResource(texture_, texture);

		D3D11_MAPPED_SUBRESOURCE mr;
		UINT sr = D3D11CalcSubresource(0, 0, 0);
		hr = GetContext()->Map(texture_, sr, D3D11_MAP_READ_WRITE, 0, &mr);
		if (FAILED(hr))
		{
			return false;
		}

		dst.resize(width * height);

		for (int32_t h = 0; h < height; h++)
		{
			auto dst_ = &(dst[h * width]);
			auto src_ = &(((uint8_t*)mr.pData)[h*mr.RowPitch]);
			memcpy(dst_, src_, width * sizeof(Color));
		}

		GetContext()->Unmap(texture_, sr);

		SafeRelease(texture_);
		return true;

	END:;
		SafeRelease(texture_);
		return false;
	}
}