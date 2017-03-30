
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

	class ConstantBuffer_Impl_DX11
		: public ConstantBuffer
	{
	private:
		ID3D11Buffer*	buffer = nullptr;

	public:
		ConstantBuffer_Impl_DX11();

		virtual ~ConstantBuffer_Impl_DX11();

		bool Initialize(Manager* manager, int32_t size);
	};

}