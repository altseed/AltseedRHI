
#include "ar.Shader_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

namespace ar
{
	Shader_Impl_GL::Shader_Impl_GL()
	{

	}

	Shader_Impl_GL::~Shader_Impl_GL()
	{
		//SafeRelease(vertexShader);
		//SafeRelease(pixelShader);
		//SafeRelease(inputLayout);
	}

	bool Shader_Impl_GL::Initialize(Manager* manager, const void* vs, int32_t vs_size, const void* ps, int32_t ps_size, const std::vector <VertexLayout>& layout)
	{
		/*
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;
		std::vector< D3D11_INPUT_ELEMENT_DESC> decl;

		hr = m->GetDevice()->CreateVertexShader(
			vs,
			vs_size,
			NULL,
			&vertexShader);

		if (FAILED(hr))
		{
			goto End;
		}

		hr = m->GetDevice()->CreatePixelShader(
			ps,
			ps_size,
			NULL,
			&pixelShader);

		if (FAILED(hr))
		{
			goto End;
		}

		int32_t byteOffset = 0;
		for each (auto& l in layout)
		{
			auto d = D3D11_INPUT_ELEMENT_DESC();
			ZeroMemory(&d, sizeof(D3D11_INPUT_ELEMENT_DESC));

			if (l.LayoutFormat == VertexLayoutFormat::R32G32B32_FLOAT)
			{
				d.SemanticName = l.Name.c_str();
				d.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				d.AlignedByteOffset = byteOffset;
				d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				byteOffset += sizeof(float) * 3;
			}
			else if (l.LayoutFormat == VertexLayoutFormat::R8G8B8A8_UNORM)
			{
				d.SemanticName = l.Name.c_str();
				d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				d.AlignedByteOffset = byteOffset;
				d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				byteOffset += sizeof(float) * 1;
			}
			else if (l.LayoutFormat == VertexLayoutFormat::R8G8B8A8_UINT)
			{
				d.SemanticName = l.Name.c_str();
				d.Format = DXGI_FORMAT_R8G8B8A8_UINT;
				d.AlignedByteOffset = byteOffset;
				d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				byteOffset += sizeof(float) * 1;
			}
			else if (l.LayoutFormat == VertexLayoutFormat::R32G32_FLOAT)
			{
				d.SemanticName = l.Name.c_str();
				d.Format = DXGI_FORMAT_R32G32_FLOAT;
				d.AlignedByteOffset = byteOffset;
				d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				byteOffset += sizeof(float) * 2;
			}
			decl.push_back(d);
		}

		hr = m->GetDevice()->CreateInputLayout(
			&(decl[0]),
			decl.size(),
			vs,
			vs_size,
			&inputLayout);

		if (FAILED(hr))
		{
			goto End;
		}

		return true;

	End:;
		SafeRelease(vertexShader);
		SafeRelease(pixelShader);
		SafeRelease(inputLayout);
		*/
		return false;
	}
}
