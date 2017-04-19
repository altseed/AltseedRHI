
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

	class IndexBuffer_Impl_DX11
		: public IndexBuffer
	{
	private:
		Manager*		manager = nullptr;
		ID3D11Buffer*	buffer = nullptr;
		

	public:
		IndexBuffer_Impl_DX11();

		virtual ~IndexBuffer_Impl_DX11();

		bool Initialize(Manager* manager, int32_t indexCount, bool is32bit) override;

		bool Write(const void* data, int32_t size) override;

		ID3D11Buffer* GetBuffer() const { return buffer; }
	};

}