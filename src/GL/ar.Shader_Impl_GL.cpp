
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

	bool Shader_Impl_GL::Initialize(Manager* manager, const void* vs, int32_t vs_size, const void* ps, int32_t ps_size, const std::vector <VertexLayout>& layout)
	{
		char* empty = "";

		char* vs_src[3];
		char* ps_src[3];
		int32_t vs_src_len[3];
		int32_t ps_src_len[3];

		int32_t vs_src_count = 0;
		int32_t ps_src_count = 0;

		GLuint vs_shader, ps_shader;
		GLint res_vs, res_ps, res_link;

		vs_src[0] = (char*)vs;
		vs_src_len[0] = strlen(vs_src[0]);

		ps_src[0] = (char*)ps;
		ps_src_len[0] = strlen(ps_src[0]);

		vs_src_count = 1;
		ps_src_count = 1;

		// Compile vertex buffer.
		vs_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs_shader, vs_src_count, (const GLchar**)vs_src, vs_src_len);
		glCompileShader(vs_shader);
		glGetShaderiv(vs_shader, GL_COMPILE_STATUS, &res_vs);

		// Compile pixel buffer.
		ps_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(ps_shader, ps_src_count, (const GLchar**)ps_src, ps_src_len);
		glCompileShader(ps_shader);
		glGetShaderiv(ps_shader, GL_COMPILE_STATUS, &res_ps);

		// Generate program
		program = glCreateProgram();
		glAttachShader(program, ps_shader);
		glAttachShader(program, vs_shader);

		// Link
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &res_link);

		if (
			res_vs == GL_FALSE ||
			res_ps == GL_FALSE ||
			res_link == GL_FALSE)
		{
			char log_text[512];
			int32_t log_size;
			glGetShaderInfoLog(vs_shader, sizeof(log_text), &log_size, log_text);
			if (log_size > 0)
			{
				OUTPUT_DEBUG_STRING("Vertex Shader:\n");
				OUTPUT_DEBUG_STRING("\n");
				OUTPUT_DEBUG_STRING(log_text);
			}
			glGetShaderInfoLog(ps_shader, sizeof(log_text), &log_size, log_text);
			if (log_size > 0)
			{
				OUTPUT_DEBUG_STRING("Fragment Shader:\n");
				OUTPUT_DEBUG_STRING("\n");
				OUTPUT_DEBUG_STRING(log_text);
			}
			glGetProgramInfoLog(program, sizeof(log_text), &log_size, log_text);
			if (log_size > 0)
			{
				OUTPUT_DEBUG_STRING("Shader Link:\n");
				OUTPUT_DEBUG_STRING(log_text);
			}

			goto Exit;
		}

		{
			char vs_text[512];
			int32_t vs_size;

			char ps_text[512];
			int32_t ps_size;

			char l_text[512];
			int32_t l_size;

			glGetShaderInfoLog(vs_shader, sizeof(vs_text), &vs_size, vs_text);
			glGetShaderInfoLog(ps_shader, sizeof(ps_text), &ps_size, ps_text);
			glGetProgramInfoLog(program, sizeof(l_text), &l_size, l_text);

			if (vs_size > 0 || ps_size > 0 || l_size > 0)
			{
				goto Exit;
			}
		}

		glDeleteShader(vs_shader);
		glDeleteShader(ps_shader);

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

		if (vs_shader != 0)
		{
			glDeleteShader(vs_shader);
			vs_shader = 0;
		}

		if (ps_shader != 0)
		{
			glDeleteShader(ps_shader);
			ps_shader = 0;
		}

		layouts.clear();

		return false;
	}
}
