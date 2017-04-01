
#include "ar.Base.h"

#if defined(_WIN32)

#include "DX11/ar.Manager_Impl_DX11.h"
#include "DX11/ar.Context_Impl_DX11.h"

#include "DX11/ar.VertexBuffer_Impl_DX11.h"
#include "DX11/ar.IndexBuffer_Impl_DX11.h"
#include "DX11/ar.ConstantBuffer_Impl_DX11.h"
#include "DX11/ar.Shader_Impl_DX11.h"

#endif

#include "GL/ar.Manager_Impl_GL.h"
#include "GL/ar.Context_Impl_GL.h"

#include "GL/ar.VertexBuffer_Impl_GL.h"
#include "GL/ar.IndexBuffer_Impl_GL.h"
#include "GL/ar.ConstantBuffer_Impl_GL.h"
#include "GL/ar.Shader_Impl_GL.h"


namespace ar
{

	Manager* Manager::Create()
	{
#if defined(_WIN32)
		return new Manager_Impl_DX11();
#endif
	}

	Context* Context::Create()
	{
#if defined(_WIN32)
		return new Context_Impl_DX11();
#endif
	}

	VertexBuffer* VertexBuffer::Create()
	{
#if defined(_WIN32)
		return new VertexBuffer_Impl_DX11();
#endif
	}

	IndexBuffer* IndexBuffer::Create()
	{
#if defined(_WIN32)
		return new IndexBuffer_Impl_DX11();
#endif
	}

	ConstantBuffer* ConstantBuffer::Create()
	{
#if defined(_WIN32)
		return new ConstantBuffer_Impl_DX11();
#endif
	}

	Shader*Shader::Create()
	{
#if defined(_WIN32)
		return new Shader_Impl_DX11();
#endif
	}
}
