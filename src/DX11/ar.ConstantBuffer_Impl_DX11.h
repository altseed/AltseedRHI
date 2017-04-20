
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

	class ConstantBuffer_Impl_DX11
		: public ConstantBuffer
	{
	private:
		Manager*		manager = nullptr;

		ID3D11Buffer*	dx_buffer = nullptr;
		std::vector<uint8_t>	buffer;


	public:
		ConstantBuffer_Impl_DX11();

		virtual ~ConstantBuffer_Impl_DX11();

		bool Initialize(Manager* manager, int32_t size) override;

		void SetData(void* data, int32_t size, int32_t offset) override
		{
			memcpy(&(buffer[offset]), data, size);
		}

		uint8_t* GetBuffer() { return buffer.data(); }
		int32_t GetSize() const { return (int32_t)buffer.size(); }

		ID3D11Buffer* GetDxBuffer() { return dx_buffer; }

		void UpdateDxBuffer();
	};

}