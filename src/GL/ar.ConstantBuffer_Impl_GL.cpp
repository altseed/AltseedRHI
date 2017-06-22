
//#if defined(_WIN32)

#include "ar.ConstantBuffer_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

namespace ar
{
	ConstantBuffer_Impl_GL::ConstantBuffer_Impl_GL()
	{

	}

	ConstantBuffer_Impl_GL::~ConstantBuffer_Impl_GL()
	{
	}

	bool ConstantBuffer_Impl_GL::Initialize(Manager* manager, int32_t size)
	{
		buffer.resize(size);
		memset(buffer.data(), 0, size);
		return true;
	}
}

//#endif
