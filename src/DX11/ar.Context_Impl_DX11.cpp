
#include "ar.Context_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"
#include "ar.VertexBuffer_Impl_DX11.h"
#include "ar.IndexBuffer_Impl_DX11.h"
#include "ar.Shader_Impl_DX11.h"

#include "ar.Texture2D_Impl_DX11.h"
#include "ar.CubeMapTexture_Impl_DX11.h"
#include "ar.RenderTexture2D_Impl_DX11.h"
#include "ar.DepthTexture_Impl_DX11.h"

#include "ar.ConstantBuffer_Impl_DX11.h"

namespace ar
{
	void Context_Impl_DX11::CreateRenderStates(Manager* manager)
	{
		auto m = (Manager_Impl_DX11*)manager;

		D3D11_CULL_MODE cullTbl[] =
		{
			D3D11_CULL_BACK,
			D3D11_CULL_FRONT,
			D3D11_CULL_NONE,
		};

		for (int32_t ct = 0; ct < CulTypeCount; ct++)
		{
			D3D11_RASTERIZER_DESC rsDesc;
			ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
			rsDesc.CullMode = cullTbl[ct];
			rsDesc.FillMode = D3D11_FILL_SOLID;
			rsDesc.DepthClipEnable = TRUE;
			m->GetDevice()->CreateRasterizerState(&rsDesc, &m_rStates[ct]);
		}

		for (int32_t dt = 0; dt < DepthTestCount; dt++)
		{
			for (int32_t dw = 0; dw < DepthWriteCount; dw++)
			{
				D3D11_DEPTH_STENCIL_DESC dsDesc;
				ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
				dsDesc.DepthEnable = dt;
				dsDesc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)dw;
				dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				dsDesc.StencilEnable = FALSE;
				m->GetDevice()->CreateDepthStencilState(&dsDesc, &m_dStates[dt][dw]);
			}
		}

		for (int32_t i = 0; i < AlphaTypeCount; i++)
		{
			D3D11_BLEND_DESC Desc;
			ZeroMemory(&Desc, sizeof(Desc));
			Desc.AlphaToCoverageEnable = false;

			for (int32_t k = 0; k < 8; k++)
			{
				Desc.RenderTarget[k].BlendEnable = i != (int32_t)AlphaBlendMode::Opacity;
				Desc.RenderTarget[k].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				Desc.RenderTarget[k].SrcBlendAlpha = D3D11_BLEND_ONE;
				Desc.RenderTarget[k].DestBlendAlpha = D3D11_BLEND_ONE;
				Desc.RenderTarget[k].BlendOpAlpha = D3D11_BLEND_OP_MAX;

				switch (i)
				{
				case (int32_t)AlphaBlendMode::Opacity:
					Desc.RenderTarget[k].DestBlend = D3D11_BLEND_ZERO;
					Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_ONE;
					Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
					break;
				case (int32_t)AlphaBlendMode::Blend:
					Desc.RenderTarget[k].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
					Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_SRC_ALPHA;
					Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
					break;
				case (int32_t)AlphaBlendMode::Add:
					Desc.RenderTarget[k].DestBlend = D3D11_BLEND_ONE;
					Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_SRC_ALPHA;
					Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
					break;
				case (int32_t)AlphaBlendMode::Sub:
					Desc.RenderTarget[k].DestBlend = D3D11_BLEND_ONE;
					Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_SRC_ALPHA;
					Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
					break;

				case (int32_t)AlphaBlendMode::Mul:
					Desc.RenderTarget[k].DestBlend = D3D11_BLEND_SRC_COLOR;
					Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_ZERO;
					Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
					break;
				case (int32_t)AlphaBlendMode::AddAll:
					Desc.RenderTarget[k].DestBlend = D3D11_BLEND_ONE;
					Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_ONE;
					Desc.RenderTarget[k].SrcBlendAlpha = D3D11_BLEND_ONE;
					Desc.RenderTarget[k].DestBlendAlpha = D3D11_BLEND_ONE;
					Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
					Desc.RenderTarget[k].BlendOpAlpha = D3D11_BLEND_OP_ADD;
					break;
				case (int32_t)AlphaBlendMode::OpacityAll:
					Desc.RenderTarget[k].DestBlend = D3D11_BLEND_ZERO;
					Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_ONE;
					Desc.RenderTarget[k].DestBlendAlpha = D3D11_BLEND_ZERO;
					Desc.RenderTarget[k].SrcBlendAlpha = D3D11_BLEND_ONE;
					Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
					Desc.RenderTarget[k].BlendOpAlpha = D3D11_BLEND_OP_ADD;
					break;
				}
			}

			m->GetDevice()->CreateBlendState(&Desc, &m_bStates[i]);
		}

		for (int32_t f = 0; f < TextureFilterCount; f++)
		{
			for (int32_t w = 0; w < TextureWrapCount; w++)
			{
				D3D11_TEXTURE_ADDRESS_MODE Addres[] = {
					D3D11_TEXTURE_ADDRESS_WRAP,
					D3D11_TEXTURE_ADDRESS_CLAMP,
				};

				D3D11_FILTER Filter[] = {
					D3D11_FILTER_MIN_MAG_MIP_POINT,
					D3D11_FILTER_MIN_MAG_MIP_LINEAR,
				};

				uint32_t Anisotropic[] = {
					0, 0,
				};

				D3D11_SAMPLER_DESC SamlerDesc = {
					Filter[f],
					Addres[w],
					Addres[w],
					Addres[w],
					0.0f,
					Anisotropic[f],
					D3D11_COMPARISON_ALWAYS,
					{ 0.0f, 0.0f, 0.0f, 0.0f },
					0.0f,
					D3D11_FLOAT32_MAX, };

				m->GetDevice()->CreateSamplerState(&SamlerDesc, &m_sStates[f][w]);
			}
		}
	}

	void Context_Impl_DX11::UpdateRenderStates(const DrawParameter& param, bool forced)
	{
		bool changeDepth = forced;
		bool changeRasterizer = forced;
		bool changeBlend = forced;

		auto& current = previousDrawParam;
		auto& next = param;

		if (current.IsDepthTest != next.IsDepthTest || forced)
		{
			changeDepth = true;
		}

		if (current.IsDepthWrite != next.IsDepthWrite || forced)
		{
			changeDepth = true;
		}

		if (changeDepth)
		{
			context->OMSetDepthStencilState(m_dStates[next.IsDepthTest][next.IsDepthWrite], 0);
		}

		if (current.Culling != next.Culling || forced)
		{
			changeRasterizer = true;
		}

		if (changeRasterizer)
		{
			context->RSSetState(m_rStates[(int32_t)next.Culling]);
		}

		if (current.AlphaBlend != next.AlphaBlend || forced)
		{
			changeBlend = true;
		}

		if (changeBlend)
		{
			float blendFactor[] = { 0, 0, 0, 0 };
			context->OMSetBlendState(m_bStates[(int32_t)next.AlphaBlend], blendFactor, 0xFFFFFFFF);
		}

		previousDrawParam = param;
	}

	Context_Impl_DX11::Context_Impl_DX11()
	{

	}

	Context_Impl_DX11::~Context_Impl_DX11()
	{
		SafeRelease(context);

		// Release states
		for (int32_t ct = 0; ct < CulTypeCount; ct++)
		{
			SafeRelease(m_rStates[ct]);
		}

		for (int32_t dt = 0; dt < DepthTestCount; dt++)
		{
			for (int32_t dw = 0; dw < DepthWriteCount; dw++)
			{
				SafeRelease(m_dStates[dt][dw]);
			}
		}

		for (int32_t i = 0; i < AlphaTypeCount; i++)
		{
			SafeRelease(m_bStates[i]);
		}

		for (int32_t f = 0; f < TextureFilterCount; f++)
		{
			for (int32_t w = 0; w < TextureWrapCount; w++)
			{
				SafeRelease(m_sStates[f][w]);
			}
		}
	}

	bool Context_Impl_DX11::Initialize(Manager* manager)
	{
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;

		this->context = m->GetContext();
		SafeAddRef(this->context);

		CreateRenderStates(manager);

		return true;

	End:;
		SafeRelease(this->context);

		return false;
	}

	void Context_Impl_DX11::Begin()
	{
		UpdateRenderStates(DrawParameter(), true);
	}

	void Context_Impl_DX11::End()
	{

	}


	void Context_Impl_DX11::Draw(const DrawParameter& param)
	{
		auto vb = (VertexBuffer_Impl_DX11*)param.VertexBufferPtr;
		auto ib = (IndexBuffer_Impl_DX11*)param.IndexBufferPtr;
		auto shader = (Shader_Impl_DX11*)param.ShaderPtr;

		UpdateRenderStates(param, false);

		// Set vertex buffer
		auto vbuf = vb->GetBuffer();
		uint32_t vertexSize = vb->GetVertexSize();
		uint32_t voffset = 0;
		context->IASetVertexBuffers(0, 1, &vbuf, &vertexSize, &voffset);
		
		// Set index buffer
		if (ib->GetIs32Bit())
		{
			context->IASetIndexBuffer(ib->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
		}
		else
		{
			context->IASetIndexBuffer(ib->GetBuffer(), DXGI_FORMAT_R16_UINT, 0);
		}

		// Set shader
		context->VSSetShader(shader->GetVertexShader(), NULL, 0);
		context->PSSetShader(shader->GetPixelShader(), NULL, 0);

		if (param.Topology == TopologyMode::Triangles)
		{
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		if (param.Topology == TopologyMode::Lines)
		{
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		}

		context->IASetInputLayout(shader->GetInputLayout());

		// set textures
		for (int32_t i = 0; i < MaxTextureCount; i++)
		{
			if (param.VertexShaderTextures[i] == nullptr)
			{
				ID3D11ShaderResourceView* srv = nullptr;
				context->VSSetShaderResources(i, 1, &srv);
			}
			else
			{
				ID3D11ShaderResourceView* srv = nullptr;

				if (param.VertexShaderTextures[i]->GetType() == TextureType::Texture2D)
				{
					auto texture = (Texture2D_Impl_DX11*)param.VertexShaderTextures[i];
					srv = texture->GetShaderResourceView();
				}
				else if (param.VertexShaderTextures[i]->GetType() == TextureType::RenderTexture2D)
				{
					auto texture = (RenderTexture2D_Impl_DX11*)param.VertexShaderTextures[i];
					srv = texture->GetShaderResourceView();
				}
				else if (param.VertexShaderTextures[i]->GetType() == TextureType::CubemapTexture)
				{
					auto texture = (CubemapTexture_Impl_DX11*)param.VertexShaderTextures[i];
					srv = texture->GetShaderResourceView();
				}
				else if (param.VertexShaderTextures[i]->GetType() == TextureType::DepthTexture)
				{
					auto texture = (DepthTexture_Impl_DX11*)param.VertexShaderTextures[i];
					srv = texture->GetShaderResourceView();
				}
				
				context->VSSetShaderResources(i, 1, &srv);

				ID3D11SamplerState* samplerTbl[] = { m_sStates[(int32_t)param.VertexShaderTextureFilers[i]][(int32_t)param.VertexShaderTextureWraps[i]] };
				context->VSSetSamplers(i, 1, samplerTbl);
			}
		}

		for (int32_t i = 0; i < MaxTextureCount; i++)
		{
			if (param.PixelShaderTextures[i] == nullptr)
			{
				ID3D11ShaderResourceView* srv = nullptr;
				context->PSSetShaderResources(i, 1, &srv);
			}
			else
			{
				ID3D11ShaderResourceView* srv = nullptr;

				if (param.PixelShaderTextures[i]->GetType() == TextureType::Texture2D)
				{
					auto texture = (Texture2D_Impl_DX11*)param.PixelShaderTextures[i];
					srv = texture->GetShaderResourceView();
				}
				else if (param.PixelShaderTextures[i]->GetType() == TextureType::RenderTexture2D)
				{
					auto texture = (RenderTexture2D_Impl_DX11*)param.PixelShaderTextures[i];
					srv = texture->GetShaderResourceView();
				}
				else if (param.PixelShaderTextures[i]->GetType() == TextureType::CubemapTexture)
				{
					auto texture = (CubemapTexture_Impl_DX11*)param.PixelShaderTextures[i];
					srv = texture->GetShaderResourceView();
				}
				else if (param.PixelShaderTextures[i]->GetType() == TextureType::DepthTexture)
				{
					auto texture = (DepthTexture_Impl_DX11*)param.PixelShaderTextures[i];
					srv = texture->GetShaderResourceView();
				}

				context->PSSetShaderResources(i, 1, &srv);

				ID3D11SamplerState* samplerTbl[] = { m_sStates[(int32_t)param.PixelShaderTextureFilers[i]][(int32_t)param.PixelShaderTextureWraps[i]] };
				context->PSSetSamplers(i, 1, samplerTbl);
			}
		}

		// constant buffer
		if (param.VertexConstantBufferPtr != nullptr)
		{
			auto cb = (ConstantBuffer_Impl_DX11*)param.VertexConstantBufferPtr;
			cb->UpdateDxBuffer();

			auto dxcb = cb->GetDxBuffer();
			context->VSSetConstantBuffers(0, 1, &dxcb);
		}

		if (param.PixelConstantBufferPtr != nullptr)
		{
			auto cb = (ConstantBuffer_Impl_DX11*)param.PixelConstantBufferPtr;
			cb->UpdateDxBuffer();

			auto dxcb = cb->GetDxBuffer();
			context->PSSetConstantBuffers(0, 1, &dxcb);
		}

		// Draw
		if (param.InstanceCount == 1)
		{
			context->DrawIndexed(
				param.IndexCount == 0 ? ib->GetIndexCount() : param.IndexCount,
				param.IndexOffset,
				param.VertexOffset);
		}
		else
		{
			context->DrawIndexedInstanced(
				param.IndexCount == 0 ? ib->GetIndexCount() : param.IndexCount,
				param.InstanceCount,
				0,
				0,
				0);
		}

	}
}