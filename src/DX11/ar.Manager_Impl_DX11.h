
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{
	class Manager_Impl_DX11
		: public Manager
	{
		ID3D11Device*			device = nullptr;
		ID3D11DeviceContext*	context = nullptr;
		IDXGIFactory1*		dxgiFactory = nullptr;
		IDXGIAdapter1*		adapter = nullptr;
		IDXGISwapChain*		swapChain = nullptr;

		int32_t				version = 0;

		ID3D11Texture2D*			defaultBack = nullptr;
		ID3D11RenderTargetView*		defaultBackRenderTargetView = nullptr;

		ID3D11Texture2D*			defaultDepthBuffer = nullptr;
		ID3D11DepthStencilView*		defaultDepthStencilView = nullptr;

		std::array<ID3D11RenderTargetView*, MaxRenderTarget>	currentBackRenderTargetViews;
		ID3D11DepthStencilView*		currentDepthStencilView = nullptr;

		void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

	public:
		Manager_Impl_DX11();
		virtual ~Manager_Impl_DX11();

		ErrorCode Initialize(const ManagerInitializationParameter& param) override;

		void BeginScene(const SceneParameter& param) override;

		void EndScene() override;

		void Present() override;

		void Clear(bool isColorTarget, bool isDepthTarget, const Color& color) override;

		bool SaveScreen(std::vector<Color>& dst, int32_t& width, int32_t& height) override;

		bool GetIsFullscreenMode() override;

		bool SetIsFullscreenMode(bool isFullscreenMode) override;

		bool SetWindowSize(int32_t width, int32_t height) override;

		std::array<void*, 2> GetInternalObjects() const override;

		bool SaveTexture(std::vector<Color>& dst, ID3D11Resource* texture, int32_t width, int32_t height);

		int32_t GetVersion() const override { return version; }

		ID3D11Device* GetDevice() const { return device; }

		ID3D11DeviceContext* GetContext() { return context; }
	};
}