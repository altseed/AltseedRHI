
#include "ar.VertexBuffer_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"

namespace ar
{
	VertexBuffer_Impl_DX11::VertexBuffer_Impl_DX11()
	{

	}

	VertexBuffer_Impl_DX11::~VertexBuffer_Impl_DX11()
	{
		SafeRelease(buffer);
	}

	bool VertexBuffer_Impl_DX11::Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount)
	{
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;
		std::vector< D3D11_INPUT_ELEMENT_DESC> decl;

		D3D11_BUFFER_DESC hBufferDesc;
		ZeroMemory(&hBufferDesc, sizeof(hBufferDesc));

		hBufferDesc.ByteWidth = vertexSize * vertexCount;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		D3D11_SUBRESOURCE_DATA hSubResourceData;
		hSubResourceData.pSysMem = NULL;
		hSubResourceData.SysMemPitch = 0;
		hSubResourceData.SysMemSlicePitch = 0;

		// Create buffer
		hr = m->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &buffer);
		if (FAILED(hr))
		{
			goto End;
		}

		this->manager = manager;
		this->vertexSize = vertexSize;
		this->vertexCount = vertexCount;

		return true;

	End:;
		SafeRelease(buffer);

		return false;
	}

	bool VertexBuffer_Impl_DX11::Write(const void* data, int32_t size)
	{
		if (size != vertexSize * vertexCount) return false;

		auto m = (Manager_Impl_DX11*)manager;

		m->GetContext()->UpdateSubresource(
			buffer,
			0,
			nullptr,
			data,
			0,
			0);

		return true;
	}
}