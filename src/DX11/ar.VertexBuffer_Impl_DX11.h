
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

	class VertexBuffer_Impl_DX11
		: public VertexBuffer
	{
	private:
		Manager*				manager = nullptr;
		ID3D11Buffer*			buffer = nullptr;

		int32_t					ringOffset = 0;
		int32_t					ringLocked = 0;

		std::vector<uint8_t>	dynamicBuffer;

	public:
		VertexBuffer_Impl_DX11();

		virtual ~VertexBuffer_Impl_DX11();

		bool Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount, bool isDynamic) override;

		bool Write(const void* data, int32_t size) override;

		void* LockRingBuffer(int32_t count) override;

		void Unlock() override;

		ID3D11Buffer* GetBuffer() const { return buffer; }
	};

}
