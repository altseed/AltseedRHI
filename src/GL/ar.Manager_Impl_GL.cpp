
#include "ar.Manager_Impl_GL.h"

namespace ar
{

	Manager_Impl_GL::Manager_Impl_GL()
	{

	}

	Manager_Impl_GL::~Manager_Impl_GL()
	{
		//SafeRelease(device);
		//SafeRelease(context);
		//SafeRelease(dxgiFactory);
		//SafeRelease(adapter);
		//SafeRelease(swapChain);
		//SafeRelease(defaultBack);
		//SafeRelease(defaultBackRenderTargetView);
	}

	ErrorCode Manager_Impl_GL::Initialize(const ManagerInitializationParameter& param)
	{
		return ErrorCode::OK;
		/*
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
		*/
	}

	void Manager_Impl_GL::BeginScene(const SceneParameter& param)
	{
		/*
		if (param.RenderTargets[0] == nullptr)
		{
			context->OMSetRenderTargets(1, &defaultBackRenderTargetView, nullptr);
		}

		// Temp
		D3D11_VIEWPORT vp;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = (float)640;
		vp.Height = (float)480;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		context->RSSetViewports(1, &vp);
		*/
	}

	void Manager_Impl_GL::EndScene()
	{

	}

	void Manager_Impl_GL::Present()
	{
    }
}
