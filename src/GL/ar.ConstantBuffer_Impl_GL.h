
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

	class ConstantBuffer_Impl_GL
		: public ConstantBuffer
	{
	private:
		std::vector<uint8_t>	buffer;

	public:
		ConstantBuffer_Impl_GL();

		virtual ~ConstantBuffer_Impl_GL();

		bool Initialize(Manager* manager, int32_t size);

		void SetData(void* data, int32_t size, const ConstantLayout& layout)
		{
			memcpy(&(buffer[layout.Offset]), data, size);
		}

		uint8_t* GetBuffer() { return buffer.data(); }
		int32_t GetSize() const { return (int32_t)buffer.size(); }
	};

}
