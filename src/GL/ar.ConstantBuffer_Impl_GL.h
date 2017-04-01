
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

	class ConstantBuffer_Impl_GL
		: public ConstantBuffer
	{
	private:
		//ID3D11Buffer*	buffer = nullptr;

	public:
		ConstantBuffer_Impl_GL();

		virtual ~ConstantBuffer_Impl_GL();

		bool Initialize(Manager* manager, int32_t size);
	};

}
