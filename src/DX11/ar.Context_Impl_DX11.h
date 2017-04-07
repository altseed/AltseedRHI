
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{
	class Context_Impl_DX11
		: public Context
	{
	private:
		ID3D11DeviceContext*	context = nullptr;
		DrawParameter			previousDrawParam;

		static const int32_t		DepthTestCount = 2;
		static const int32_t		DepthWriteCount = 2;
		static const int32_t		CulTypeCount = 3;
		static const int32_t		AlphaTypeCount = 7;
		static const int32_t		TextureFilterCount = 2;
		static const int32_t		TextureWrapCount = 2;

		ID3D11RasterizerState*		m_rStates[CulTypeCount];
		ID3D11DepthStencilState*	m_dStates[DepthTestCount][DepthWriteCount];
		ID3D11BlendState*			m_bStates[AlphaTypeCount];
		ID3D11SamplerState*			m_sStates[TextureFilterCount][TextureWrapCount];

		void CreateRenderStates(Manager* manager);
		void UpdateRenderStates(const DrawParameter& param, bool forced);
	public:
		Context_Impl_DX11();

		virtual ~Context_Impl_DX11();

		bool Initialize(Manager* manager) override;

		void Begin() override;

		void End() override;

		void Draw(const DrawParameter& param) override;
	};

}