
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
		currentBackRenderTargetViews.fill(nullptr);
	}

	Manager_Impl_DX11::~Manager_Impl_DX11()
	{
		BOOL isScreenMode = FALSE;
		swapChain->GetFullscreenState(&isScreenMode, 0);
		if (isScreenMode)
		{
			swapChain->SetFullscreenState(FALSE, 0);
		}

		for (auto& v : currentBackRenderTargetViews)
		{
			SafeRelease(v);
		}
		SafeRelease(currentDepthStencilView);

		SafeRelease(device);
		SafeRelease(context);
		SafeRelease(dxgiFactory);
		SafeRelease(adapter);
		SafeRelease(swapChain);
		SafeRelease(defaultBack);
		SafeRelease(defaultBackRenderTargetView);

		SafeRelease(defaultDepthBuffer);
		SafeRelease(defaultDepthStencilView);

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

		// Create depth buffer
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = param.WindowWidth;
		descDepth.Height = param.WindowHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;


		if (FAILED(device->CreateTexture2D(&descDepth, NULL, &defaultDepthBuffer)))
		{
			result = ErrorCode::FailedToCreateDepthBuffer;
			goto End;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
		viewDesc.Format = descDepth.Format;
		viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		viewDesc.Flags = 0;
		if (FAILED(device->CreateDepthStencilView(defaultDepthBuffer, &viewDesc, &defaultDepthStencilView)))
		{
			result = ErrorCode::FailedToCreateDepthBufferView;
			goto End;
		}

		for (auto a : adapters)
		{
			a->Release();
		}

		windowWidth = param.WindowWidth;
		windowHeight = param.WindowHeight;
		colorSpaceType = param.ColorSpace;

		if (currentFeatureLevel == D3D_FEATURE_LEVEL_11_0) version = 110;
		if (currentFeatureLevel == D3D_FEATURE_LEVEL_10_1) version = 101;
		if (currentFeatureLevel == D3D_FEATURE_LEVEL_10_0) version = 100;
		if (currentFeatureLevel == D3D_FEATURE_LEVEL_9_3) version = 93;
		if (currentFeatureLevel == D3D_FEATURE_LEVEL_9_2) version = 92;
		if (currentFeatureLevel == D3D_FEATURE_LEVEL_9_1) version = 91;

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
		SafeRelease(defaultDepthBuffer);
		SafeRelease(defaultDepthStencilView);

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
			context->OMSetRenderTargets(1, &defaultBackRenderTargetView, defaultDepthStencilView);
			SetViewport(0, 0, windowWidth, windowHeight);

			// Reset current buffers
			for (auto& v : currentBackRenderTargetViews)
			{
				SafeRelease(v);
			}
			SafeRelease(currentDepthStencilView);

			currentBackRenderTargetViews[0] = defaultBackRenderTargetView;
			currentDepthStencilView = defaultDepthStencilView;

			SafeAddRef(currentBackRenderTargetViews[0]);
			SafeAddRef(currentDepthStencilView);
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

			// Reset current buffers
			for (auto& v : currentBackRenderTargetViews)
			{
				SafeRelease(v);
			}
			SafeRelease(currentDepthStencilView);

			for (int32_t i = 0; i < MaxRenderTarget; i++)
			{
				currentBackRenderTargetViews[i] = rt[i];
				SafeAddRef(currentBackRenderTargetViews[i]);
			}
			currentDepthStencilView = ds;
			
			SafeAddRef(currentDepthStencilView);
		}

	}

	void Manager_Impl_DX11::EndScene()
	{

	}

	void Manager_Impl_DX11::Present()
	{
		// Not sync
		swapChain->Present(0, 0);
	}

	void Manager_Impl_DX11::Clear(bool isColorTarget, bool isDepthTarget, const Color& color)
	{
		if (isColorTarget)
		{
			float ClearColor[] = { color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };

			for (auto i = 0; i < MaxRenderTarget; i++)
			{
				if (currentBackRenderTargetViews[i] == nullptr) continue;
				context->ClearRenderTargetView(currentBackRenderTargetViews[i], ClearColor);
			}
		}

		if (isDepthTarget && currentDepthStencilView != nullptr)
		{
			context->ClearDepthStencilView(currentDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	bool Manager_Impl_DX11::SaveScreen(std::vector<Color>& dst, int32_t& width, int32_t& height)
	{
		ID3D11Resource* resource = nullptr;

		defaultBackRenderTargetView->GetResource(&resource);
		auto ret = SaveTexture(dst, resource, windowWidth, windowHeight);

		SafeRelease(resource);

		width = windowWidth;
		height = windowHeight;

		return ret;
	}

	bool Manager_Impl_DX11::GetIsFullscreenMode()
	{
		BOOL isScreenMode = FALSE;
		swapChain->GetFullscreenState(&isScreenMode, 0);
		return isScreenMode;
	}

	bool Manager_Impl_DX11::SetIsFullscreenMode(bool isFullscreenMode)
	{
		BOOL isScreenMode = FALSE;
		swapChain->GetFullscreenState(&isScreenMode, 0);

		if (isScreenMode && isFullscreenMode) return true;
		if (!isScreenMode && !isFullscreenMode) return true;

		if (isFullscreenMode)
		{
			swapChain->SetFullscreenState(TRUE, 0);
		}
		else
		{
			swapChain->SetFullscreenState(FALSE, 0);
		}

		

		return true;
	}

	bool Manager_Impl_DX11::SetWindowSize(int32_t width, int32_t height)
	{
		// Reset
		context->OMSetRenderTargets(1, &defaultBackRenderTargetView, defaultDepthStencilView);
		SafeRelease(defaultBack);
		SafeRelease(defaultBackRenderTargetView);
		SafeRelease(defaultDepthBuffer);
		SafeRelease(defaultDepthStencilView);

		for (auto& v : currentBackRenderTargetViews)
		{
			SafeRelease(v);
		}
		SafeRelease(currentDepthStencilView);

		windowWidth = width;
		windowHeight = height;

		DXGI_FORMAT format;

		if (colorSpaceType == ColorSpaceType::LinearSpace)
		{
			format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		}
		else
		{
			format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		swapChain->ResizeBuffers(1, windowWidth, windowHeight, format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&defaultBack);
		device->CreateRenderTargetView(defaultBack, NULL, &defaultBackRenderTargetView);

		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = windowWidth;
		descDepth.Height = windowHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		device->CreateTexture2D(&descDepth, NULL, &defaultDepthBuffer);

		D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
		viewDesc.Format = descDepth.Format;
		viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		viewDesc.Flags = 0;
		device->CreateDepthStencilView(defaultDepthBuffer, &viewDesc, &defaultDepthStencilView);

		context->OMSetRenderTargets(1, &defaultBackRenderTargetView, defaultDepthStencilView);

		currentBackRenderTargetViews[0] = defaultBackRenderTargetView;
		currentDepthStencilView = defaultDepthStencilView;

		SafeAddRef(currentBackRenderTargetViews[0]);
		SafeAddRef(currentDepthStencilView);

		return true;
	}

	std::array<void*, 2> Manager_Impl_DX11::GetInternalObjects() const
	{
		std::array<void*, 2> ret;
		ret[0] = device;
		ret[1] = context;
		return ret;
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