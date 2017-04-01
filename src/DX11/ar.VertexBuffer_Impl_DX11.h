
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

	class VertexBuffer_Impl_DX11
		: public VertexBuffer
	{
	private:
		Manager*		manager = nullptr;
		ID3D11Buffer*	buffer = nullptr;

	public:
		VertexBuffer_Impl_DX11();

		virtual ~VertexBuffer_Impl_DX11();

		bool Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount) override;

		bool Write(const void* data, int32_t size) override;

		ID3D11Buffer* GetBuffer() const { return buffer; }
	};

}
