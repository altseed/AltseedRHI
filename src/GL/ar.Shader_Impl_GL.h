
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

	class Shader_Impl_GL
		: public Shader
	{
	private:
		//ID3D11VertexShader*			vertexShader = nullptr;
		//ID3D11PixelShader*			pixelShader = nullptr;
		//ID3D11InputLayout*			inputLayout = nullptr;

	public:
		Shader_Impl_GL();

		virtual ~Shader_Impl_GL();

		bool Initialize(Manager* manager, const void* vs, int32_t vs_size, const void* ps, int32_t ps_size, const std::vector <VertexLayout>& layout) override;

		//ID3D11VertexShader* GetVertexShader() const { return vertexShader; }
		//ID3D11PixelShader* GetPixelShader() const { return pixelShader; }
		//ID3D11InputLayout* GetInputLayout() const { return inputLayout; }

	};

}
