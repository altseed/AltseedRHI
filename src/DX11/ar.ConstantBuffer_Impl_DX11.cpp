#include "ar.ConstantBuffer_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"

namespace ar
{
	ConstantBuffer_Impl_DX11::ConstantBuffer_Impl_DX11()
	{

	}

	ConstantBuffer_Impl_DX11::~ConstantBuffer_Impl_DX11()
	{
		SafeRelease(dx_buffer);
	}

	bool ConstantBuffer_Impl_DX11::Initialize(Manager* manager, int32_t size)
	{
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;
		std::vector< D3D11_INPUT_ELEMENT_DESC> decl;

		D3D11_BUFFER_DESC hBufferDesc;
		ZeroMemory(&hBufferDesc, sizeof(hBufferDesc));

		hBufferDesc.ByteWidth = size;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		D3D11_SUBRESOURCE_DATA hSubResourceData;
		hSubResourceData.pSysMem = NULL;
		hSubResourceData.SysMemPitch = 0;
		hSubResourceData.SysMemSlicePitch = 0;

		// Create buffer
		hr = m->GetDevice()->CreateBuffer(&hBufferDesc, NULL, &dx_buffer);
		if (FAILED(hr))
		{
			goto End;
		}

		buffer.resize(size);
		memset(buffer.data(), 0, size);

		this->manager = manager;

		return true;

	End:;
		SafeRelease(dx_buffer);

		return false;
	}

	void ConstantBuffer_Impl_DX11::UpdateDxBuffer()
	{
		auto m = (Manager_Impl_DX11*)manager;
		m->GetContext()->UpdateSubresource(dx_buffer, 0, NULL, buffer.data(), 0, 0);
	}
}

