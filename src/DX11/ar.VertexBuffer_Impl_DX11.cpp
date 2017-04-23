
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

	bool VertexBuffer_Impl_DX11::Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount, bool isDynamic)
	{
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;
		std::vector< D3D11_INPUT_ELEMENT_DESC> decl;

		D3D11_BUFFER_DESC hBufferDesc;
		ZeroMemory(&hBufferDesc, sizeof(hBufferDesc));

		hBufferDesc.ByteWidth = vertexSize * vertexCount;
		hBufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		hBufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
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
		this->isDynamic = isDynamic;

		if (this->isDynamic)
		{
			this->dynamicBuffer.resize(vertexSize * vertexCount);
		}
		
		return true;

	End:;
		SafeRelease(buffer);

		return false;
	}

	bool VertexBuffer_Impl_DX11::Write(const void* data, int32_t size)
	{
		if (size != vertexSize * vertexCount) return false;

		auto m = (Manager_Impl_DX11*)manager;

		if (isDynamic)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			m->GetContext()->Map(
				buffer,
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&mappedResource);

			memcpy(mappedResource.pData, data, vertexSize * vertexCount);

			m->GetContext()->Unmap(buffer, 0);
		}
		else
		{
			m->GetContext()->UpdateSubresource(
				buffer,
				0,
				nullptr,
				data,
				0,
				0);
		}

		ringOffset = 0;
		vertexOffset = ringOffset;

		return true;
	}

	void* VertexBuffer_Impl_DX11::LockRingBuffer(int32_t count)
	{
		if (count > vertexCount) return nullptr;
		if (!isDynamic) return nullptr;

		if (ringOffset + count >= vertexCount)
		{
			ringOffset = 0;
		}
		
		auto p = &(dynamicBuffer[ringOffset * vertexSize]);

		ringLocked = count;
		vertexOffset = ringOffset;

		return p;
	}

	void VertexBuffer_Impl_DX11::Unlock()
	{
		auto m = (Manager_Impl_DX11*)manager;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		m->GetContext()->Map(
			buffer,
			0,
			ringOffset != 0 ? D3D11_MAP_WRITE_NO_OVERWRITE : D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedResource);

		uint8_t* dst = (uint8_t*)mappedResource.pData;
		dst += (ringOffset * vertexSize);

		uint8_t* src = (uint8_t*)dynamicBuffer.data();
		src += (ringOffset * vertexSize);

		memcpy(dst, src, ringLocked * vertexSize);

		m->GetContext()->Unmap(buffer, 0);

		ringOffset += ringLocked;
	}
}