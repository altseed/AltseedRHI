
#include "ar.Shader_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

namespace ar
{
#if _WIN32
#include <Windows.h>
#define OUTPUT_DEBUG_STRING(s)	OutputDebugStringA(s)
#else
#define OUTPUT_DEBUG_STRING(s)	printf("%s\n",s);
#endif

	void Shader_Impl_GL::Reflect(
		std::map<std::string, ConstantLayout>& dst_constant,
		int32_t& constantSize,
		std::map<std::string, TextureLayout>& dst_texture,
		int32_t& textureCount,
		GLuint program)
	{
		int32_t uniformCount = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);

		int32_t offset = 0;

		for (int32_t u = 0; u < uniformCount; u++)
		{
			char name[256];
			int32_t nameLen = 0;
			GLint size = 0;
			GLenum type;
			glGetActiveUniform(program, u, sizeof(name), &nameLen, &size, &type, name);

			if (type == GL_SAMPLER_2D)
			{
				TextureLayout layout;
				layout.ID = glGetUniformLocation(program, name);
				layout.Index = dst_texture.size();
				dst_texture[name] = layout;

			}
			else if (type == GL_SAMPLER_CUBE)
			{
				TextureLayout layout;
				layout.ID = glGetUniformLocation(program, name);
				layout.Index = dst_texture.size();
				dst_texture[name] = layout;
			}
			else
			{
				std::string name_ = name;

				ConstantLayout l;
				l.Index = glGetUniformLocation(program, name);
				l.Offset = offset;
				l.Count = size;

				if (type == GL_FLOAT)
				{
					l.Type = ConstantBufferFormat::Float1;
					offset += sizeof(float) * 1 * l.Count;
				}
				else if (type == GL_FLOAT_VEC2)
				{
					l.Type = ConstantBufferFormat::Float2;
					offset += sizeof(float) * 2 * l.Count;
				}
				else if (type == GL_FLOAT_VEC3)
				{
					l.Type = ConstantBufferFormat::Float3;
					offset += sizeof(float) * 3 * l.Count;
				}
				else if (type == GL_FLOAT_VEC4)
				{
					if (l.Count > 1)
					{
						l.Type = ConstantBufferFormat::Float4_ARRAY;
						offset += sizeof(float) * 4 * l.Count;

						auto result = name_.find_first_of("[");
						if (result != std::string::npos)
						{
							name_ = name_.substr(0, result);
						}
					}
					else
					{
						l.Type = ConstantBufferFormat::Float4;
						offset += sizeof(float) * 4 * l.Count;
					}
				}
				else if (type == GL_FLOAT_MAT4)
				{
					if (l.Count > 1)
					{
						l.Type = ConstantBufferFormat::Matrix44_ARRAY;
						offset += sizeof(float) * 16 * l.Count;

						auto result = name_.find_first_of("[");
						if (result != std::string::npos)
						{
							name_ = name_.substr(0, result);
						}
					}
					else
					{
						l.Type = ConstantBufferFormat::Matrix44;
						offset += sizeof(float) * 16 * l.Count;
					}
				}
				else
				{
					printf("unknown\n");
					continue;
				}

				dst_constant[name_] = l;
			}
		}

		constantSize = offset;
		textureCount = dst_texture.size();
	}

	Shader_Impl_GL::Shader_Impl_GL()
	{

	}

	Shader_Impl_GL::~Shader_Impl_GL()
	{
		if (program != 0)
		{
			glDeleteProgram(program);
			program = 0;
		}
	}

	bool Shader_Impl_GL::Initialize(Manager* manager, const ShaderCompilerResult& compilerResult, const std::vector <VertexLayout>& layout, bool is32bit)
	{
		program = compilerResult.ID;
		uint16_t byteOffset = 0;

		for (auto& l : layout)
		{
			auto att = glGetAttribLocation(program, l.Name.c_str());
			Layout il;
			il.attribute = att;

			if (l.LayoutFormat == VertexLayoutFormat::R32G32B32_FLOAT)
			{
				il.type = GL_FLOAT;
				il.count = 3;
				il.normalized = false;
				il.offset = byteOffset;
				byteOffset += sizeof(float) * 3;
			}
			else if (l.LayoutFormat == VertexLayoutFormat::R8G8B8A8_UNORM)
			{
				il.type = GL_UNSIGNED_BYTE;
				il.count = 4;
				il.normalized = true;
				il.offset = byteOffset;
				byteOffset += sizeof(float) * 1;
			}
			else if (l.LayoutFormat == VertexLayoutFormat::R8G8B8A8_UINT)
			{
				il.type = GL_UNSIGNED_BYTE;
				il.count = 4;
				il.normalized = false;
				il.offset = byteOffset;
				byteOffset += sizeof(float) * 1;
			}
			else if (l.LayoutFormat == VertexLayoutFormat::R32G32_FLOAT)
			{
				il.type = GL_FLOAT;
				il.count = 2;
				il.normalized = false;
				il.offset = byteOffset;
				byteOffset += sizeof(float) * 2;
			}

			layouts.push_back(il);
		}

		Reflect(
			pixelConstantLayouts,
			pixelConstantBufferSize,
			pixelTextureLayouts,
			pixelTextureCount,
			program);

		return true;

	Exit:;

		if (program != 0)
		{
			glDeleteProgram(program);
			program = 0;
		}

		layouts.clear();

		return false;
	}
}
