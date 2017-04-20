#include "ar.Compiler_Impl_DX11.h"

namespace ar
{
	static ID3DBlob* CompileVertexShader(const char* vertexShaderText, const char* vertexShaderFileName, const std::vector <D3D_SHADER_MACRO>& macro, bool isDX9Mode, std::string& log)
	{
		ID3DBlob* shader = nullptr;
		ID3DBlob* error = nullptr;

		UINT flag = D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;
#if !_DEBUG
		flag = flag | D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

		HRESULT hr;

		if (isDX9Mode)
		{
			hr = D3DCompile(
				vertexShaderText,
				strlen(vertexShaderText),
				vertexShaderFileName,
				macro.size() > 0 ? (D3D_SHADER_MACRO*)&macro[0] : NULL,
				NULL,
				"main",
				"vs_4_0_level_9_3",
				flag,
				0,
				&shader,
				&error);
		}
		else
		{
			/* この方法だとVS4.0以上でのコンパイルが要求 */
			hr = D3DCompile(
				vertexShaderText,
				strlen(vertexShaderText),
				vertexShaderFileName,
				macro.size() > 0 ? (D3D_SHADER_MACRO*)&macro[0] : NULL,
				NULL,
				"main",
				"vs_4_0",
				flag,
				0,
				&shader,
				&error);
		}

		if (FAILED(hr))
		{
			if (hr == E_OUTOFMEMORY)
			{
				log += "Out of memory\n";
			}
			else
			{
				log += "Unknown error\n";
			}

			if (error != NULL)
			{
				log += (const char*)error->GetBufferPointer();
				error->Release();
			}

			return nullptr;
		}

		return shader;
	}

	static ID3DBlob* CompilePixelShader(const char* vertexShaderText, const char* vertexShaderFileName, const std::vector <D3D_SHADER_MACRO>& macro, bool isDX9Mode, std::string& log)
	{
		ID3DBlob* shader = nullptr;
		ID3DBlob* error = nullptr;


		UINT flag = D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;
#if !_DEBUG
		flag = flag | D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

		HRESULT hr;

		if (isDX9Mode)
		{
			hr = D3DCompile(
				vertexShaderText,
				strlen(vertexShaderText),
				vertexShaderFileName,
				macro.size() > 0 ? (D3D_SHADER_MACRO*)&macro[0] : NULL,
				NULL,
				"main",
				"ps_4_0_level_9_3",
				flag,
				0,
				&shader,
				&error);
		}
		else
		{
			/* この方法だとPS4.0以上でのコンパイルが要求 */
			hr = D3DCompile(
				vertexShaderText,
				strlen(vertexShaderText),
				vertexShaderFileName,
				macro.size() > 0 ? (D3D_SHADER_MACRO*)&macro[0] : NULL,
				NULL,
				"main",
				"ps_4_0",
				flag,
				0,
				&shader,
				&error);
		}

		if (FAILED(hr))
		{
			if (hr == E_OUTOFMEMORY)
			{
				log += "Out of memory\n";
			}
			else
			{
				log += "Unknown error\n";
			}

			if (error != NULL)
			{
				log += (const char*)error->GetBufferPointer();
				error->Release();
			}

			return nullptr;
		}

		return shader;
	}


	Compiler_Impl_DX11::Compiler_Impl_DX11()
	{

	}

	Compiler_Impl_DX11::~Compiler_Impl_DX11()
	{

	}

	bool Compiler_Impl_DX11::Compile(ShaderCompilerResult& result, ShaderCompilerParameter& param)
	{
		std::vector <D3D_SHADER_MACRO> macros;

		for (auto& m : param.Macros)
		{
			D3D_SHADER_MACRO m_;
			m_.Name = m.Name.c_str();
			m_.Definition = m.Definition.c_str();
			macros.push_back(m_);
		}

		if (macros.size() > 0)
		{
			D3D_SHADER_MACRO m_;
			m_.Definition = NULL;
			m_.Name = NULL;
			macros.push_back(m_);
		}

		std::string log;

		auto vs = CompileVertexShader(param.VertexShaderTexts[0].c_str(), "", macros, false, log);
		auto ps = CompilePixelShader(param.PixelShaderTexts[0].c_str(), "", macros, false, log);

		if (vs != nullptr && ps != nullptr)
		{
			result.ErrorMessage = log;
			
			result.VertexShaderBuffer.resize(vs->GetBufferSize());
			memcpy(result.VertexShaderBuffer.data(), vs->GetBufferPointer(), vs->GetBufferSize());

			result.PixelShaderBuffer.resize(ps->GetBufferSize());
			memcpy(result.PixelShaderBuffer.data(), ps->GetBufferPointer(), ps->GetBufferSize());

			SafeRelease(vs);
			SafeRelease(ps);

			return true;
		}

		SafeRelease(vs);
		SafeRelease(ps);

		result.ErrorMessage = log;

		return false;
	}
}