#pragma once

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <array>
#include <string>
#include <vector>
#include <map>

namespace ar
{
	const int32_t MaxRenderTarget = 4;
	const int32_t MaxTextureCount = 16;

	typedef void(*PNGLoadFunc)(const uint8_t* data, int32_t width, int32_t height, void* userData);

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
		FailedToCreateDepthBuffer,
		FailedToCreateDepthBufferView,
		FailedToInitializeGlew,
	};

	template <typename T>
	inline void SafeDelete(T*& p)
	{
		if (p != NULL)
		{
			delete (p);
			(p) = NULL;
		}
	}

	template <typename T>
	inline void SafeDeleteArray(T*& p)
	{
		if (p != NULL)
		{
			delete[](p);
			(p) = NULL;
		}
	}


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

	enum class TextureType : int32_t
	{
		Texture2D = 0,
		RenderTexture2D = 1,
		CubemapTexture = 2,
		DepthTexture = 3,
	};

	enum class TopologyMode : int32_t
	{
		Triangles,
		Lines,
	};
	/**
	@brief	描画時のブレンドモードを表す列挙体
	*/
	enum class AlphaBlendMode : int32_t
	{
		/// <summary>
		/// 不透明
		/// </summary>
		Opacity = 0,
		/// <summary>
		/// 透明
		/// </summary>
		Blend = 1,
		/// <summary>
		/// 加算
		/// </summary>
		Add = 2,
		/// <summary>
		/// 減算
		/// </summary>
		Sub = 3,
		/// <summary>
		/// 乗算
		/// </summary>
		Mul = 4,
		/// <summary>
		/// 全加算(内部処理用)
		/// </summary>
		AddAll = 5,
		/// <summary>
		/// 全不透明(内部処理用)
		/// </summary>
		OpacityAll = 6,
	};

	enum class TextureFilterType : int32_t
	{
		Nearest = 0,
		Linear = 1,
	};

	enum class TextureWrapType : int32_t
	{
		Repeat = 0,
		Clamp = 1,
	};

	enum class CullingType : int32_t
	{
		Front = 0,
		Back = 1,
		Double = 2,
	};

	enum class VertexLayoutFormat : int32_t
	{
		R32G32B32_FLOAT,
		R8G8B8A8_UNORM,
		R8G8B8A8_UINT,
		R32G32_FLOAT,
	};

	enum class ConstantBufferFormat : int32_t
	{
		Float1,
		Float2,
		Float3,
		Float4,
		Float4_ARRAY,
		Matrix44,
		Matrix44_ARRAY,
		Unknown,
	};

	enum class OpenGLVersionType : int32_t
	{
		None,
		OpenGL21,
		OpenGL33,
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

	struct TextureLayout
	{
		int32_t					Index;

		/**
			@brief	This is used only OpenGL.
		*/
		int32_t					ID = 0;
	};

	struct ConstantLayout
	{
		int32_t					Index;
		ConstantBufferFormat	Type;
		int32_t			Offset;
		int32_t			Count;

		int32_t GetSize() const
		{
			if (Type == ConstantBufferFormat::Float1) return sizeof(float) * 1;
			if (Type == ConstantBufferFormat::Float2) return sizeof(float) * 2;
			if (Type == ConstantBufferFormat::Float3) return sizeof(float) * 3;
			if (Type == ConstantBufferFormat::Float4) return sizeof(float) * 4;
			if (Type == ConstantBufferFormat::Float4_ARRAY) return sizeof(float) * 4 * Count;
			if (Type == ConstantBufferFormat::Matrix44) return sizeof(float) * 16;
			if (Type == ConstantBufferFormat::Matrix44_ARRAY) return sizeof(float) * 16 * Count;
			return 0;
		}
	};

	class Manager;
	class Context;
	class Compiler;

	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
	class Shader;

	class Texture;
	class Texture2D;
	class RenderTexture2D;
	class DepthTexture;

	struct Color
	{
		uint8_t	R;
		uint8_t	G;
		uint8_t	B;
		uint8_t	A;
	};

	struct TextureLockInfomation
	{
		void*		Pixels = nullptr;
		int32_t		Pitch = 0;
		int32_t		Width = 0;
		int32_t		Height = 0;
	};

	struct ManagerInitializationParameter
	{
		std::array<void*, 2>	Handles;
		int32_t					WindowWidth = 0;
		int32_t					WindowHeight = 0;

		bool				IsFullscreenMode = false;
		ColorSpaceType		ColorSpace = ColorSpaceType::GammaSpace;

	};

	struct ShaderMacro
	{
		std::string Name;
		std::string Definition;

		ShaderMacro()
		{
		}

		ShaderMacro(const char* name, const char* definition)
			: Name(name)
			, Definition(definition)
		{
		}
	};

	struct ShaderCompilerParameter
	{
		OpenGLVersionType			OpenGLVersion = OpenGLVersionType::None;
		std::vector<ShaderMacro>	Macros;
		std::vector<std::string>	VertexShaderTexts;
		std::vector<std::string>	PixelShaderTexts;
	};

	struct ShaderCompilerResult
	{
		std::string				ErrorMessage;
		int32_t					ID = 0;
		std::vector<uint8_t>	VertexShaderBuffer;
		std::vector<uint8_t>	PixelShaderBuffer;
	};

	struct SceneParameter
	{
		std::array<RenderTexture2D*, MaxRenderTarget> RenderTargets;
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

		ConstantBuffer*	VertexConstantBufferPtr = nullptr;
		ConstantBuffer*	PixelConstantBufferPtr = nullptr;

		std::array<Texture*, MaxTextureCount>	VertexShaderTextures;
		std::array<TextureFilterType, MaxTextureCount>	VertexShaderTextureFilers;
		std::array<TextureWrapType, MaxTextureCount>	VertexShaderTextureWraps;

		std::array<Texture*, MaxTextureCount>	PixelShaderTextures;
		std::array<TextureFilterType, MaxTextureCount>	PixelShaderTextureFilers;
		std::array<TextureWrapType, MaxTextureCount>	PixelShaderTextureWraps;

		AlphaBlendMode	AlphaBlend = AlphaBlendMode::Blend;
		bool			IsDepthTest = false;
		bool			IsDepthWrite = false;
		CullingType		Culling = CullingType::Front;
		TopologyMode	Topology = TopologyMode::Triangles;

		int32_t			VertexOffset = 0;
		int32_t			IndexOffset = 0;
		int32_t			IndexCount = 0;

		int32_t			InstanceCount = 1;

		DrawParameter()
		{
			VertexShaderTextures.fill(nullptr);
			VertexShaderTextureFilers.fill(TextureFilterType::Linear);
			VertexShaderTextureWraps.fill(TextureWrapType::Clamp);

			PixelShaderTextures.fill(nullptr);
			PixelShaderTextureFilers.fill(TextureFilterType ::Linear);
			PixelShaderTextureWraps.fill(TextureWrapType::Clamp);
		}
	};

	class Manager
	{
	protected:
		int32_t	windowWidth = 0;
		int32_t	windowHeight = 0;
		ColorSpaceType		colorSpaceType = ColorSpaceType::GammaSpace;
		GraphicsDeviceType	deviceType = GraphicsDeviceType::Default;

	public:

		Manager() {}
		virtual ~Manager() {}

		virtual ErrorCode Initialize(const ManagerInitializationParameter& param) { return ErrorCode::Unknown; }
		virtual void BeginRendering() {}
		virtual void EndRendering() {}

		virtual void BeginScene(const SceneParameter& param) {}
		virtual void EndScene() {}

		virtual void Present() {}

		/**
			@brief	Clear current render tagets.
			@note
			Context::Begin is required after executing Manager::Clear.
		*/
		virtual void Clear(bool isColorTarget, bool isDepthTarget, const Color& color) {}

		virtual bool SaveScreen(std::vector<Color>& dst, int32_t& width, int32_t& height) { return false; }

		virtual bool GetIsFullscreenMode() { return false; }

		virtual bool SetIsFullscreenMode(bool isFullscreenMode) { return false; }

		virtual bool SetWindowSize(int32_t width, int32_t height) { return false; }

		virtual std::array<void*, 2> GetInternalObjects() const { return std::array<void*, 2>(); }

		/**
			@brief	allocate buffer from GPU.
		*/
		virtual uint8_t* Alloc(int32_t size, int32_t alignment) { return nullptr; }

		/**
			@brief	free buffer from GPU.
		*/
		virtual void Free(const uint8_t* buffer) { return; }

		/**
			@brief	Get version(Directxx, OpenGLxx, etc)
		*/
		virtual int32_t GetVersion() const { return 0; }

		GraphicsDeviceType GetDeviceType() const { return deviceType; }

		static Manager* Create(GraphicsDeviceType device);
	};

	class Context
	{
	public:
		Context() {}
		virtual ~Context() {}

		virtual bool Initialize(Manager* manager) { return true; }

		virtual void Begin() {}

		virtual void End() {}

		virtual void Draw(const DrawParameter& param) {}

		static Context* Create(Manager* manager);
	};
	
	class Compiler
	{
	public:
		Compiler() = default;

		virtual ~Compiler() = default;

		virtual bool Compile(ShaderCompilerResult& result, ShaderCompilerParameter& param) { return false; }

		static Compiler* Create(Manager* manager);
	};

	class VertexBuffer
	{
	protected:
		int32_t			vertexSize = 0;
		int32_t			vertexCount = 0;
		int32_t			vertexOffset = 0;

		bool			isDynamic = false;
		
	public:
		VertexBuffer() {}
		virtual ~VertexBuffer() {}

		virtual bool Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount, bool isDynamic) { return false; }

		virtual bool Write(const void* data, int32_t size) { return false; }

		virtual void* LockRingBuffer(int32_t count) { return nullptr; }

		virtual void Unlock() {}

		int32_t GetVertexCount() const { return vertexCount; }

		int32_t GetVertexSize() const { return vertexSize; }

		int32_t GetVertexOffset() const { return vertexOffset; }

		static VertexBuffer* Create(Manager* manager);
	};

	class IndexBuffer
	{
	protected:
		int32_t			indexCount = 0;
		bool			is32bit = false;

	public:
		IndexBuffer() {}
		virtual ~IndexBuffer() {}

		virtual bool Initialize(Manager* manager, int32_t indexCount, bool is32bit) { return false; }

		virtual bool Write(const void* data, int32_t size) { return false; }

		int32_t GetIndexCount() const { return indexCount; }

		bool GetIs32Bit() const { return is32bit; }

		static IndexBuffer* Create(Manager* manager);
	};


	class ConstantBuffer
	{
	public:
		ConstantBuffer() {}
		virtual ~ConstantBuffer() {}
		
		virtual bool Initialize(Manager* manager, int32_t size) { return false; }

		virtual void SetData(void* data, int32_t size, int32_t offset) {}

		static ConstantBuffer* Create(Manager* manager);
	};

	class Shader
	{
	protected:

		std::map<std::string, ConstantLayout>	vertexConstantLayouts;
		std::map<std::string, TextureLayout>	vertexTextureLayouts;
		int32_t	vertexTextureCount = 0;
		int32_t	vertexConstantBufferSize = 0;

		std::map<std::string, ConstantLayout>	pixelConstantLayouts;
		std::map<std::string, TextureLayout>	pixelTextureLayouts;
		int32_t	pixelTextureCount = 0;
		int32_t	pixelConstantBufferSize = 0;

	public:
		Shader() {}
		virtual ~Shader() {}

		virtual bool Initialize(Manager* manager, const ShaderCompilerResult& compilerResult, const std::vector <VertexLayout>& layout, bool is32bit) { return false; }

		int32_t GetVertexConstantBufferSize() const { return vertexConstantBufferSize; }

		int32_t GetPixelConstantBufferSize() const { return pixelConstantBufferSize; }

		int32_t GetVertexTextureCount() const { return vertexTextureCount; }

		int32_t GetPixelTextureCount() const { return pixelTextureCount; }

		std::map<std::string, ConstantLayout>& GetVertexConstantLayouts()
		{
			return vertexConstantLayouts;
		}

		std::map<std::string, ConstantLayout>& GetPixelConstantLayouts()
		{
			return pixelConstantLayouts;
		}

		std::map<std::string, TextureLayout>& GetVertexTextureLayouts()
		{
			return vertexTextureLayouts;
		}

		std::map<std::string, TextureLayout>& GetPixelTextureLayouts()
		{
			return pixelTextureLayouts;
		}

		ConstantLayout* GetVertexConstantLayout(const char* name)
		{
			auto it = vertexConstantLayouts.find(name);

			if (it != vertexConstantLayouts.end())
			{
				return &(it->second);
			}

			return nullptr;
		}

		ConstantLayout* GetPixelConstantLayout(const char* name)
		{
			auto it = pixelConstantLayouts.find(name);

			if (it != pixelConstantLayouts.end())
			{
				return &(it->second);
			}

			return nullptr;
		}

		int32_t GetVertexShaderTextureIndex(const char* name)
		{
			auto it = vertexTextureLayouts.find(name);

			if (it != vertexTextureLayouts.end())
			{
				return it->second.Index;
			}

			return -1;
		}

		int32_t GetPixelShaderTextureIndex(const char* name)
		{
			auto it = pixelTextureLayouts.find(name);

			if (it != pixelTextureLayouts.end())
			{
				return it->second.Index;
			}

			return -1;
		}

		static Shader* Create(Manager* manager);
	};

	class Texture
	{
	protected:
		TextureFormat	format = TextureFormat::R8_UNORM;

	public:
		Texture() {}
		virtual ~Texture() {}

		TextureFormat GetFormat() const { return format; }

		virtual TextureType GetType() const = 0;
	};

	class Texture2D
		: public Texture
	{
	protected:
		int32_t	width = 0;
		int32_t height = 0;
	public:
		Texture2D() {}
		virtual ~Texture2D() {}

		virtual bool Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format, void* data, bool isEditable) { return false; }

		virtual bool Initialize(Manager* manager, const void* src, int32_t src_size, bool isEditable, bool isSRGB) { return false; }

		virtual bool Lock(TextureLockInfomation* info) { return false; }

		virtual void Unlock() {}

		virtual bool Save(std::vector<Color>& dst, int32_t& width, int32_t& height) { return false; }

		int32_t GetWidth() const { return width; }

		int32_t GetHeight() const { return height; }

		TextureType GetType() const override { return TextureType::Texture2D; }

		virtual std::array<uint64_t, 2> GetInternalObjects() const { return std::array<uint64_t, 2>(); }

		static Texture2D* Create(Manager* manager);
	};

	class RenderTexture2D
		: public Texture2D
	{
	public:
		RenderTexture2D() {}
		virtual ~RenderTexture2D() {}

		virtual bool Initialize(Manager* manager, int32_t width, int32_t height, TextureFormat format) { return false; }

		TextureType GetType() const override { return TextureType::RenderTexture2D; }

		static RenderTexture2D* Create(Manager* manager);
	};

	class DepthTexture
		: public Texture2D
	{
	public:
		DepthTexture() {}
		virtual ~DepthTexture() {}

		virtual bool Initialize(Manager* manager, int32_t width, int32_t height) { return false; }

		TextureType GetType() const override { return TextureType::DepthTexture; }

		static DepthTexture* Create(Manager* manager);
	};

	class CubemapTexture
		: public Texture
	{
	protected:
		int32_t width = 0;
		int32_t height = 0;
		int32_t	mipmapCount = 0;
		
	public:
		CubemapTexture() {}
		virtual ~CubemapTexture() {}

		virtual bool Initialize(Manager* manager, void* data, int32_t size) { return false; }

		int32_t GetWidth() const { return width; }

		int32_t GetHeight() const { return height; }

		int32_t GetMipmapCount() const { return mipmapCount; }

		TextureType GetType() const override { return TextureType::CubemapTexture; }

		static CubemapTexture* Create(Manager* manager);
	};
}




namespace ar
{
class ImageHelper
{
	static int32_t	reference;
public:

	static void Initizlize();

	static void Terminate();

	static bool SavePNG(const char16_t* path, int32_t width, int32_t height, const void* data);
	static bool LoadPNG(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size);

	/**
	@brief	1ピクセルのサイズを取得する。
	@param	format	フォーマット
	@return	サイズ
	*/
	static int32_t GetPitch(TextureFormat format);

	/**
	@brief	画像が消費する推定VRAMサイズを取得する。
	*/
	static int32_t GetVRAMSize(TextureFormat format, int32_t width, int32_t height);

	static int32_t GetMipmapCount(int32_t width, int32_t height);
	static void GetMipmapSize(int mipmap, int32_t& width, int32_t& height);

	static bool IsPNG(const void* data, int32_t size);
	static bool IsDDS(const void* data, int32_t size);

	static std::string GetFormatName(TextureFormat format);
};

}
