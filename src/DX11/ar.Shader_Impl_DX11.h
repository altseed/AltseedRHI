
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

	class Shader_Impl_DX11
		: public Shader
	{
	private:
		ID3D11VertexShader*			vertexShader = nullptr;
		ID3D11PixelShader*			pixelShader = nullptr;
		ID3D11InputLayout*			inputLayout = nullptr;

		void Reflect(
			std::map<std::string, ConstantLayout>& dst_constant,
			int32_t& constantSize,
			std::map<std::string, TextureLayout>& dst_texture,
			int32_t& textureCount, 
			void* buf, int32_t bufSize);
	public:
		Shader_Impl_DX11();

		virtual ~Shader_Impl_DX11();

		bool Initialize(Manager* manager, const ShaderCompilerResult& compilerResult, const std::vector <VertexLayout>& layout, bool is32bit) override;

		ID3D11VertexShader* GetVertexShader() const { return vertexShader; }
		ID3D11PixelShader* GetPixelShader() const { return pixelShader; }
		ID3D11InputLayout* GetInputLayout() const { return inputLayout; }

	};

}