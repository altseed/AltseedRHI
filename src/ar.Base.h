
#pragma once

#include <stdint.h>

#include <array>
#include <string>
#include <vector>

namespace ar
{
	enum class ErrorCode : int32_t
	{
		OK,
		Unknown,
		FailedToCreateFactory,
		FailedToGetAdapter,
		FailedToCreateDevice,
		FailedToCreateSwapChain,
		FailedToGetBackBuffer,
		FailedToCreateBackBufferTarget,
	};

	template <class T>
	void SafeAddRef(T& t)
	{
		if (t != NULL)
		{
			t->AddRef();
		}
	}

	template <class T>
	void SafeRelease(T& t)
	{
		if (t != NULL)
		{
			t->Release();
			t = NULL;
		}
	}

	enum class GraphicsDeviceType : int32_t
	{
		Default,
		DirectX11,
		OpenGL,
	};

	enum class ColorSpaceType : int32_t
	{
		GammaSpace = 0,	///< ガンマスペース
		LinearSpace = 1, ///< リニアスペース
	};

	enum class TextureFormat : int32_t
	{
		R8G8B8A8_UNORM = 0,
		R16G16B16A16_FLOAT = 11,
		R32G32B32A32_FLOAT = 1,
		R8G8B8A8_UNORM_SRGB = 2,
		R16G16_FLOAT = 3,
		R8_UNORM = 4,

		BC1 = 5,
		BC2 = 6,
		BC3 = 7,
		BC1_SRGB = 8,
		BC2_SRGB = 9,
		BC3_SRGB = 10,

	};

	enum class VertexLayoutFormat : int32_t
	{
		R32G32B32_FLOAT,
		R8G8B8A8_UNORM,
		R8G8B8A8_UINT,
		R32G32_FLOAT,
	};

	struct VertexLayout
	{
		std::string			Name;
		VertexLayoutFormat LayoutFormat;

		VertexLayout()
		{
			Name = "";
			LayoutFormat = VertexLayoutFormat::R32G32B32_FLOAT;
		}

		VertexLayout(const char* name, VertexLayoutFormat format)
		{
			Name = name;
			LayoutFormat = format;
		}
	};

	class Manager;
	class Context;

	class VertexBuffer;
	class IndexBuffer;
	class Shader;

	class Texture;
	class Texture2D;
	class RenderTexture2D;
	class DepthTexture;


	struct ManagerInitializationParameter
	{
		std::array<void*, 2>	Handles;
		int32_t					WindowWidth = 0;
		int32_t					WindowHeight = 0;

		bool				IsFullscreenMode = false;
		ColorSpaceType		ColorSpace = ColorSpaceType::GammaSpace;

	};

	struct SceneParameter
	{
		std::array<RenderTexture2D*, 4> RenderTargets;
		DepthTexture*	DepthTarget = nullptr;

		SceneParameter()
		{
			RenderTargets.fill(nullptr);
		}
	};

	struct DrawParameter
	{
		Shader*			ShaderPtr = nullptr;
		VertexBuffer*	VertexBufferPtr = nullptr;
		IndexBuffer*	IndexBufferPtr = nullptr;
	};

	class Texture
	{
	public:
		Texture() {}
		virtual ~Texture() {}
	};

	class Texture2D
		: public Texture
	{
	public:
		Texture2D() {}
		virtual ~Texture2D() {}
	};

	class RenderTexture2D
		: public Texture2D
	{
	public:
		RenderTexture2D() {}
		virtual ~RenderTexture2D() {}
	};

	class DepthTexture
		: public Texture2D
	{
	public:
		DepthTexture() {}
		virtual ~DepthTexture() {}
	};


	class Manager
	{
	private:

	public:

		Manager() {}
		virtual ~Manager() {}

		virtual ErrorCode Initialize(const ManagerInitializationParameter& param) { return ErrorCode::Unknown; }
		virtual void BeginRendering() {}
		virtual void EndRendering() {}

		virtual void BeginScene(const SceneParameter& param) {}
		virtual void EndScene() {}

		virtual void Present() {}

		static Manager* Create();
	};

	class Context
	{
	public:
		Context() {}
		virtual ~Context() {}

		virtual bool Initialize(Manager* manager) { return true; }

		virtual void Draw(const DrawParameter& param) {}

		static Context* Create();
	};

	class VertexBuffer
	{
	protected:
		int32_t			vertexSize = 0;
		int32_t			vertexCount = 0;

	public:
		VertexBuffer() {}
		virtual ~VertexBuffer() {}

		virtual bool Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount) { return false; }

		virtual bool Write(const void* data, int32_t size) { return false; }

		int32_t GetVertexCount() const { return vertexCount; }

		int32_t GetVertexSize() const { return vertexSize; }

		static VertexBuffer* Create();
	};

	class IndexBuffer
	{
	protected:
		int32_t			indexCount = 0;

	public:
		IndexBuffer() {}
		virtual ~IndexBuffer() {}

		virtual bool Initialize(Manager* manager, int32_t indexCount) { return false; }

		virtual bool Write(const void* data, int32_t size) { return false; }

		int32_t GetIndexCount() const { return indexCount; }

		static IndexBuffer* Create();
	};


	class ConstantBuffer
	{
	public:
		ConstantBuffer() {}
		virtual ~ConstantBuffer() {}

		static ConstantBuffer* Create();
	};

	class Shader
	{
	public:
		Shader() {}
		virtual ~Shader() {}

		virtual bool Initialize(Manager* manager, const void* vs, int32_t vs_size, const void* ps, int32_t ps_size, const std::vector <VertexLayout>& layout) { return false; }

		static Shader* Create();
	};



}
