
#include "ar.Base.h"

#if defined(_PSVITA)

#include "PSVITA/ar.Manager_Impl_PSVITA.h"
#include "PSVITA/ar.Context_Impl_PSVITA.h"

#include "PSVITA/ar.VertexBuffer_Impl_PSVITA.h"
#include "PSVITA/ar.IndexBuffer_Impl_PSVITA.h"
#include "PSVITA/ar.ConstantBuffer_Impl_PSVITA.h"
#include "PSVITA/ar.Shader_Impl_PSVITA.h"

#include "PSVITA/ar.Texture2D_Impl_PSVITA.h"
#include "PSVITA/ar.RenderTexture2D_Impl_PSVITA.h"
#include "PSVITA/ar.DepthTexture_Impl_PSVITA.h"
#include "PSVITA/ar.CubemapTexture_Impl_PSVITA.h"

#elif defined(_PS4)

#include "PS4/ar.Manager_Impl_PS4.h"
#include "PS4/ar.Context_Impl_PS4.h"

#include "PS4/ar.VertexBuffer_Impl_PS4.h"
#include "PS4/ar.IndexBuffer_Impl_PS4.h"
#include "PS4/ar.ConstantBuffer_Impl_PS4.h"
#include "PS4/ar.Shader_Impl_PS4.h"

#include "PS4/ar.Texture2D_Impl_PS4.h"
#include "PS4/ar.RenderTexture2D_Impl_PS4.h"
#include "PS4/ar.DepthTexture_Impl_PS4.h"
#include "PS4/ar.CubemapTexture_Impl_PS4.h"

#elif defined(_SWITCH)

#include "SWITCH/ar.Manager_Impl_SWITCH.h"
#include "SWITCH/ar.Context_Impl_SWITCH.h"

#include "SWITCH/ar.VertexBuffer_Impl_SWITCH.h"
#include "SWITCH/ar.IndexBuffer_Impl_SWITCH.h"
#include "SWITCH/ar.ConstantBuffer_Impl_SWITCH.h"
#include "SWITCH/ar.Shader_Impl_SWITCH.h"

#include "SWITCH/ar.Texture2D_Impl_SWITCH.h"
#include "SWITCH/ar.RenderTexture2D_Impl_SWITCH.h"
#include "SWITCH/ar.DepthTexture_Impl_SWITCH.h"
#include "SWITCH/ar.CubemapTexture_Impl_SWITCH.h"

#elif defined(_XBOXONE)

#include "XBOXONE/ar.Manager_Impl_XBOXONE.h"
#include "XBOXONE/ar.Context_Impl_XBOXONE.h"

#include "XBOXONE/ar.VertexBuffer_Impl_XBOXONE.h"
#include "XBOXONE/ar.IndexBuffer_Impl_XBOXONE.h"
#include "XBOXONE/ar.ConstantBuffer_Impl_XBOXONE.h"
#include "XBOXONE/ar.Shader_Impl_XBOXONE.h"

#include "XBOXONE/ar.Texture2D_Impl_XBOXONE.h"
#include "XBOXONE/ar.RenderTexture2D_Impl_XBOXONE.h"
#include "XBOXONE/ar.DepthTexture_Impl_XBOXONE.h"
#include "XBOXONE/ar.CubemapTexture_Impl_XBOXONE.h"

#elif defined(_WIN32)

#include "DX11/ar.Manager_Impl_DX11.h"
#include "DX11/ar.Context_Impl_DX11.h"
#include "DX11/ar.Compiler_Impl_DX11.h"

#include "DX11/ar.VertexBuffer_Impl_DX11.h"
#include "DX11/ar.IndexBuffer_Impl_DX11.h"
#include "DX11/ar.ConstantBuffer_Impl_DX11.h"
#include "DX11/ar.Shader_Impl_DX11.h"

#include "DX11/ar.Texture2D_Impl_DX11.h"
#include "DX11/ar.RenderTexture2D_Impl_DX11.h"
#include "DX11/ar.DepthTexture_Impl_DX11.h"
#include "DX11/ar.CubemapTexture_Impl_DX11.h"

#include "GL/ar.Manager_Impl_GL.h"
#include "GL/ar.Context_Impl_GL.h"
#include "GL/ar.Compiler_Impl_GL.h"

#include "GL/ar.VertexBuffer_Impl_GL.h"
#include "GL/ar.IndexBuffer_Impl_GL.h"
#include "GL/ar.ConstantBuffer_Impl_GL.h"
#include "GL/ar.Shader_Impl_GL.h"

#include "GL/ar.Texture2D_Impl_GL.h"
#include "GL/ar.RenderTexture2D_Impl_GL.h"
#include "GL/ar.DepthTexture_Impl_GL.h"
#include "GL/ar.CubemapTexture_Impl_GL.h"

#else

#include "GL/ar.Manager_Impl_GL.h"
#include "GL/ar.Context_Impl_GL.h"
#include "GL/ar.Compiler_Impl_GL.h"

#include "GL/ar.VertexBuffer_Impl_GL.h"
#include "GL/ar.IndexBuffer_Impl_GL.h"
#include "GL/ar.ConstantBuffer_Impl_GL.h"
#include "GL/ar.Shader_Impl_GL.h"

#include "GL/ar.Texture2D_Impl_GL.h"
#include "GL/ar.RenderTexture2D_Impl_GL.h"
#include "GL/ar.DepthTexture_Impl_GL.h"
#include "GL/ar.CubemapTexture_Impl_GL.h"
#endif

namespace ar
{

	Manager* Manager::Create(GraphicsDeviceType device)
	{
#if defined(_PSVITA)
		return new Manager_Impl_PSVITA();
#elif defined(_PS4)
		return new Manager_Impl_PS4();
#elif defined(_SWITCH)
		return new Manager_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new Manager_Impl_XBOXONE();
#elif defined(_WIN32)
		if (device == GraphicsDeviceType::DirectX11)
		{
			return new Manager_Impl_DX11();
		}
		return new Manager_Impl_GL();
#else
		return new Manager_Impl_GL();
#endif

	}

	Context* Context::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new Context_Impl_PSVITA();
#elif defined(_PS4)
		return new Context_Impl_PS4();
#elif defined(_SWITCH)
		return new Context_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new Context_Impl_XBOXONE();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new Context_Impl_DX11();
		}
		return new Context_Impl_GL();
#else
		return new Context_Impl_GL();
#endif
	}

	Compiler* Compiler::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new Compiler();
#elif defined(_PS4)
		return new Compiler();
#elif defined(_SWITCH)
		return new Compiler();
#elif defined(_XBOXONE)
		return new Compiler();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new Compiler_Impl_DX11();
		}
		return new Compiler_Impl_GL();
#else
		return new Compiler_Impl_GL();
#endif
	}

	VertexBuffer* VertexBuffer::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new VertexBuffer_Impl_PSVITA();
#elif defined(_PS4)
		return new VertexBuffer_Impl_PS4();
#elif defined(_SWITCH)
		return new VertexBuffer_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new VertexBuffer_Impl_XBOXONE();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new VertexBuffer_Impl_DX11();
		}
		return new VertexBuffer_Impl_GL();
#else
		return new VertexBuffer_Impl_GL();
#endif
	}

	IndexBuffer* IndexBuffer::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new IndexBuffer_Impl_PSVITA();
#elif defined(_PS4)
		return new IndexBuffer_Impl_PS4();
#elif defined(_SWITCH)
		return new IndexBuffer_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new IndexBuffer_Impl_XBOXONE();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new IndexBuffer_Impl_DX11();
		}
		return new IndexBuffer_Impl_GL();
#else
		return new IndexBuffer_Impl_GL();
#endif
	}

	ConstantBuffer* ConstantBuffer::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new ConstantBuffer_Impl_PSVITA();
#elif defined(_PS4)
		return new ConstantBuffer_Impl_PS4();
#elif defined(_SWITCH)
		return new ConstantBuffer_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new ConstantBuffer_Impl_XBOXONE();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new ConstantBuffer_Impl_DX11();
		}
		return new ConstantBuffer_Impl_GL();
#else
		return new ConstantBuffer_Impl_GL();
#endif
	}

	Shader* Shader::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new Shader_Impl_PSVITA();
#elif defined(_PS4)
		return new Shader_Impl_PS4();
#elif defined(_SWITCH)
		return new Shader_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new Shader_Impl_XBOXONE();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new Shader_Impl_DX11();
		}
		return new Shader_Impl_GL();
#else
		return new Shader_Impl_GL();
#endif
	}

	Texture2D*Texture2D::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new Texture2D_Impl_PSVITA();
#elif defined(_PS4)
		return new Texture2D_Impl_PS4();
#elif defined(_SWITCH)
		return new Texture2D_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new Texture2D_Impl_XBOXONE();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new Texture2D_Impl_DX11();
		}
		return new Texture2D_Impl_GL();
#else
		return new Texture2D_Impl_GL();
#endif
	}

	RenderTexture2D* RenderTexture2D::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new RenderTexture2D_Impl_PSVITA();
#elif defined(_PS4)
		return new RenderTexture2D_Impl_PS4();
#elif defined(_SWITCH)
		return new RenderTexture2D_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new RenderTexture2D_Impl_XBOXONE();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new RenderTexture2D_Impl_DX11();
		}
		return new RenderTexture2D_Impl_GL();
#else
		return new RenderTexture2D_Impl_GL();
#endif
	}

	DepthTexture* DepthTexture::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new DepthTexture_Impl_PSVITA();
#elif defined(_PS4)
		return new DepthTexture_Impl_PS4();
#elif defined(_SWITCH)
		return new DepthTexture_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new DepthTexture_Impl_XBOXONE();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new DepthTexture_Impl_DX11();
		}
		return new DepthTexture_Impl_GL();
#else
		return new DepthTexture_Impl_GL();
#endif
	}

	CubemapTexture* CubemapTexture::Create(Manager* manager)
	{
#if defined(_PSVITA)
		return new CubemapTexture_Impl_PSVITA();
#elif defined(_PS4)
		return new CubemapTexture_Impl_PS4();
#elif defined(_SWITCH)
		return new CubemapTexture_Impl_SWITCH();
#elif defined(_XBOXONE)
		return new CubemapTexture_Impl_XBOXONE();
#elif defined(_WIN32)
		if (manager->GetDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new CubemapTexture_Impl_DX11();
		}
		return new CubemapTexture_Impl_GL();
#else
		return new CubemapTexture_Impl_GL();
#endif
	}
}
