
#include "ar.IndexBuffer_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"

namespace ar
{
	IndexBuffer_Impl_DX11::IndexBuffer_Impl_DX11()
	{

	}

	IndexBuffer_Impl_DX11::~IndexBuffer_Impl_DX11()
	{
		SafeRelease(buffer);
	}

	bool IndexBuffer_Impl_DX11::Initialize(Manager* manager, int32_t indexCount, bool is32bit)
	{
		int32_t isize = is32bit ? 4 : 2;

		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;
		std::vector< D3D11_INPUT_ELEMENT_DESC> decl;

		D3D11_BUFFER_DESC hBufferDesc;
		ZeroMemory(&hBufferDesc, sizeof(hBufferDesc));

		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.ByteWidth = isize * indexCount;
		hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = isize;

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
		this->indexCount = indexCount;
		this->is32bit = is32bit;

		return true;

	End:;
		SafeRelease(buffer);

		return false;
	}

	bool IndexBuffer_Impl_DX11::Write(const void* data, int32_t size)
	{
		int32_t isize = is32bit ? 4 : 2;

		if (size != indexCount * isize) return false;

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