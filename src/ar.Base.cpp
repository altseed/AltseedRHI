
#include "ar.Base.h"

#include "DirectX11/ar.Manager_Impl_DX11.h"
#include "DirectX11/ar.Context_Impl_DX11.h"

#include "DirectX11/ar.VertexBuffer_Impl_DX11.h"
#include "DirectX11/ar.IndexBuffer_Impl_DX11.h"
#include "DirectX11/ar.ConstantBuffer_Impl_DX11.h"
#include "DirectX11/ar.Shader_Impl_DX11.h"

namespace ar
{

	Manager* Manager::Create()
	{
		return new Manager_Impl_DX11();
	}


	Context* Context::Create()
	{
		return new Context_Impl_DX11();
	}

	VertexBuffer* VertexBuffer::Create()
	{
		return new VertexBuffer_Impl_DX11();
	}

	IndexBuffer* IndexBuffer::Create()
	{
		return new IndexBuffer_Impl_DX11();
	}

	ConstantBuffer* ConstantBuffer::Create()
	{
		return new ConstantBuffer_Impl_DX11();
	}

	Shader*Shader::Create()
	{
		return new Shader_Impl_DX11();
	}
}