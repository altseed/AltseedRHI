#define NOMINMAX

#include "ar.Shader_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"

namespace ar
{
	void Shader_Impl_DX11::Reflect(
		std::map<std::string, ConstantLayout>& dst_constant,
		int32_t& constantSize,
		std::map<std::string, TextureLayout>& dst_texture,
		int32_t& textureCount,
		void* buf, int32_t bufSize)
	{
		auto getBufferType = [](D3D11_SHADER_TYPE_DESC typeDesc, ConstantBufferFormat& format, int32_t& elements) -> void
		{
			elements = 1;

			if (typeDesc.Class == D3D_SHADER_VARIABLE_CLASS::D3D10_SVC_SCALAR && typeDesc.Type == D3D_SHADER_VARIABLE_TYPE::D3D10_SVT_FLOAT)
			{
				if (typeDesc.Columns == 1) format = ConstantBufferFormat::Float1;
			}

			if (typeDesc.Class == D3D_SHADER_VARIABLE_CLASS::D3D_SVC_VECTOR && typeDesc.Type == D3D_SHADER_VARIABLE_TYPE::D3D10_SVT_FLOAT)
			{
				if (typeDesc.Columns == 2) format = ConstantBufferFormat::Float2;
				if (typeDesc.Columns == 3) format = ConstantBufferFormat::Float3;

				else if (typeDesc.Elements > 0)
				{
					if (typeDesc.Columns == 4)
					{
						elements = typeDesc.Elements;
						format = ConstantBufferFormat::Float4_ARRAY;
					}
				}
				else
				{
					if (typeDesc.Columns == 4) format = ConstantBufferFormat::Float4;
				}
			}

			if (typeDesc.Class == D3D_SHADER_VARIABLE_CLASS::D3D_SVC_MATRIX_ROWS && typeDesc.Type == D3D_SHADER_VARIABLE_TYPE::D3D10_SVT_FLOAT)
			{
				if (typeDesc.Rows == 4 && typeDesc.Columns == 4)
				{
					if (typeDesc.Elements == 0) format = ConstantBufferFormat::Matrix44;
					if (typeDesc.Elements > 0)
					{
						elements = typeDesc.Elements;
						format = ConstantBufferFormat::Matrix44_ARRAY;
					}
				}
			}
		};

		auto getResourceType = [](D3D11_SHADER_INPUT_BIND_DESC bindDesc, ConstantBufferFormat& format, int32_t& bindPoint) -> bool
		{
			if (bindDesc.Type == D3D_SIT_TEXTURE)
			{
				if (bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURE2D)
				{
					bindPoint = bindDesc.BindPoint;
					return true;
				}
				else if (bindDesc.Dimension == D3D_SRV_DIMENSION_TEXTURECUBE)
				{
					bindPoint = bindDesc.BindPoint;
					return true;
				}
				else
				{
					return false;
				}
			}

			return false;
		};

		int32_t offset = 0;

		ID3D11ShaderReflection*	reflection = nullptr;
		D3DReflect(buf, bufSize, IID_ID3D11ShaderReflection, (void**)&reflection);

		D3D11_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);

		for (int32_t i = 0; i < shaderDesc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			reflection->GetResourceBindingDesc(i, &bindDesc);

			ConstantBufferFormat format;
			int32_t bindPoint = 0;
			if (!getResourceType(bindDesc, format, bindPoint)) continue;
			auto name = bindDesc.Name;

			TextureLayout layout;
			layout.Index = bindPoint;

			dst_texture[name] = layout;
		}

		for (int32_t i = 0; i < shaderDesc.ConstantBuffers; i++)
		{
			auto cbuf = reflection->GetConstantBufferByIndex(i);

			D3D11_SHADER_BUFFER_DESC bufferDesc;
			cbuf->GetDesc(&bufferDesc);
			for (int32_t j = 0; j < bufferDesc.Variables; j++)
			{
				auto variable = cbuf->GetVariableByIndex(j);
				if (variable == nullptr) continue;

				auto type = variable->GetType();

				D3D11_SHADER_TYPE_DESC typeDesc;
				type->GetDesc(&typeDesc);

				D3D11_SHADER_VARIABLE_DESC variableDesc;
				variable->GetDesc(&variableDesc);
				auto name = variableDesc.Name;
				auto size = variableDesc.Size;
				auto startOffset = variableDesc.StartOffset;

				ConstantBufferFormat format;
				int32_t elements = 0;
				getBufferType(typeDesc, format, elements);

				offset = std::max(offset, (int32_t)(startOffset + size));
				ConstantLayout l;
				l.Count = elements;
				l.Offset = startOffset;
				l.Type = format;

				dst_constant[name] = l;
			}

		}

		SafeRelease(reflection);

		constantSize = offset;
		textureCount = dst_texture.size();
	}

	Shader_Impl_DX11::Shader_Impl_DX11()
	{

	}

	Shader_Impl_DX11::~Shader_Impl_DX11()
	{
		SafeRelease(vertexShader);
		SafeRelease(pixelShader);
		SafeRelease(inputLayout);
	}

	bool Shader_Impl_DX11::Initialize(Manager* manager, const ShaderCompilerResult& compilerResult, const std::vector <VertexLayout>& layout, bool is32bit)
	{
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;
		std::vector< D3D11_INPUT_ELEMENT_DESC> decl;

		hr = m->GetDevice()->CreateVertexShader(
			compilerResult.VertexShaderBuffer.data(),
			compilerResult.VertexShaderBuffer.size(),
			NULL,
			&vertexShader);

		if (FAILED(hr))
		{
			goto End;
		}

		hr = m->GetDevice()->CreatePixelShader(
			compilerResult.PixelShaderBuffer.data(),
			compilerResult.PixelShaderBuffer.size(),
			NULL,
			&pixelShader);

		if (FAILED(hr))
		{
			goto End;
		}

		int32_t byteOffset = 0;
		for each (auto& l in layout)
		{
			auto d = D3D11_INPUT_ELEMENT_DESC();
			ZeroMemory(&d, sizeof(D3D11_INPUT_ELEMENT_DESC));

			if (l.LayoutFormat == VertexLayoutFormat::R32G32B32_FLOAT)
			{
				d.SemanticName = l.Name.c_str();
				d.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				d.AlignedByteOffset = byteOffset;
				d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				byteOffset += sizeof(float) * 3;
			}
			else if (l.LayoutFormat == VertexLayoutFormat::R8G8B8A8_UNORM)
			{
				d.SemanticName = l.Name.c_str();
				d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				d.AlignedByteOffset = byteOffset;
				d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				byteOffset += sizeof(float) * 1;
			}
			else if (l.LayoutFormat == VertexLayoutFormat::R8G8B8A8_UINT)
			{
				d.SemanticName = l.Name.c_str();
				d.Format = DXGI_FORMAT_R8G8B8A8_UINT;
				d.AlignedByteOffset = byteOffset;
				d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				byteOffset += sizeof(float) * 1;
			}
			else if (l.LayoutFormat == VertexLayoutFormat::R32G32_FLOAT)
			{
				d.SemanticName = l.Name.c_str();
				d.Format = DXGI_FORMAT_R32G32_FLOAT;
				d.AlignedByteOffset = byteOffset;
				d.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				byteOffset += sizeof(float) * 2;
			}
			decl.push_back(d);
		}

		hr = m->GetDevice()->CreateInputLayout(
			&(decl[0]),
			decl.size(),
			compilerResult.VertexShaderBuffer.data(),
			compilerResult.VertexShaderBuffer.size(),
			&inputLayout);

		if (FAILED(hr))
		{
			goto End;
		}

		Reflect(
			vertexConstantLayouts,
			vertexConstantBufferSize,
			vertexTextureLayouts,
			vertexTextureCount,
			(void*)compilerResult.VertexShaderBuffer.data(),
			compilerResult.VertexShaderBuffer.size());

		Reflect(
			pixelConstantLayouts,
			pixelConstantBufferSize,
			pixelTextureLayouts,
			pixelTextureCount,
			(void*)compilerResult.PixelShaderBuffer.data(),
			compilerResult.PixelShaderBuffer.size());

		return true;

	End:;
		SafeRelease(vertexShader);
		SafeRelease(pixelShader);
		SafeRelease(inputLayout);

		return false;
	}
}