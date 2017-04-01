
#include "ar.Shader_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

namespace ar
{
	Shader_Impl_GL::Shader_Impl_GL()
	{

	}

	Shader_Impl_GL::~Shader_Impl_GL()
	{
		glDeleteProgram(program);
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

		GLuint program, vs_shader, ps_shader;
		GLint res_vs, res_ps, res_link;

		vs_src[0] = (char*)vs;
		vs_src_len[0] = strlen(vs_src[0]);

		ps_src[1] = (char*)ps;
		ps_src_len[1] = strlen(ps_src[1]);

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

		return true;

	Exit:;

		glDeleteProgram(program);
		glDeleteShader(vs_shader);
		glDeleteShader(ps_shader);
		layouts.clear();

		return false;
	}
}
