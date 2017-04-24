
#include "ar.Compiler_Impl_GL.h"

namespace ar
{
	static GLuint CompileShader(std::vector<const char*>& vs_src, std::vector<const char*>& ps_src, std::string& log)
	{
		std::vector<int32_t> vs_src_len;
		std::vector<int32_t> ps_src_len;
		GLint res_vs, res_ps, res_link;

		for (auto src : vs_src)
		{
			vs_src_len.push_back(strlen(src));
		}

		for (auto src : ps_src)
		{
			ps_src_len.push_back(strlen(src));
		}
		
		// Compile vertex shader
		auto vs_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs_shader, vs_src.size(), (const GLchar**)vs_src.data(), vs_src_len.data());
		glCompileShader(vs_shader);
		glGetShaderiv(vs_shader, GL_COMPILE_STATUS, &res_vs);

		// Compile fragment shader
		auto ps_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(ps_shader, ps_src.size(), (const GLchar**)ps_src.data(), ps_src_len.data());
		glCompileShader(ps_shader);
		glGetShaderiv(ps_shader, GL_COMPILE_STATUS, &res_ps);

		// Generate program
		auto program = glCreateProgram();
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
				log += "Vertex Shader:\n";
				log += log_text;
				log += "\n";
			}
			glGetShaderInfoLog(ps_shader, sizeof(log_text), &log_size, log_text);
			if (log_size > 0)
			{
				log += "Fragment Shader:\n";
				log += log_text;
				log += "\n";
			}
			glGetProgramInfoLog(program, sizeof(log_text), &log_size, log_text);
			if (log_size > 0)
			{
				log += "Shader Link:\n";
				log += log_text;
				log += "\n";
			}

			glDeleteProgram(program);
			glDeleteShader(vs_shader);
			glDeleteShader(ps_shader);

			return 0;
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
				if (vs_size > 0)
				{
					log += "Vertex Shader:\n";
					log += vs_text;
					log += "\n";
				}

				if (ps_size > 0)
				{
					log += "Fragment Shader:\n";
					log += ps_text;
					log += "\n";
				}

				if (l_size > 0)
				{
					log += "Shader Link:\n";
					log += l_text;
					log += "\n";
				}
			}
		}

		glDeleteShader(vs_shader);
		glDeleteShader(ps_shader);
		GLCheckError();

		return program;
	}

	Compiler_Impl_GL::Compiler_Impl_GL()
	{

	}

	Compiler_Impl_GL::~Compiler_Impl_GL()
	{

	}

	bool Compiler_Impl_GL::Compile(ShaderCompilerResult& result, ShaderCompilerParameter& param)
	{
		std::vector<const char*> vs_src;
		std::vector<const char*> ps_src;

		auto gl_none = "";
		auto gl_gl21 = "#version 120\r\n";
		auto gl_gl33 = "#version 330\r\n";

		if (param.OpenGLVersion == OpenGLVersionType::None)
		{
			vs_src.push_back(gl_none);
			ps_src.push_back(gl_none);
		}

		if (param.OpenGLVersion == OpenGLVersionType::OpenGL21)
		{
			vs_src.push_back(gl_gl21);
			ps_src.push_back(gl_gl21);
		}

		if (param.OpenGLVersion == OpenGLVersionType::OpenGL33)
		{
			vs_src.push_back(gl_gl33);
			ps_src.push_back(gl_gl33);
		}

		std::string macros;

		for (auto m : param.Macros)
		{
			macros += ("#define " + m.Name + " " + m.Definition + "\r\n");
		}

		vs_src.push_back(macros.c_str());
		ps_src.push_back(macros.c_str());

		for (auto& t : param.VertexShaderTexts)
		{
			vs_src.push_back((char*)t.c_str());
		}

		for (auto& t : param.PixelShaderTexts)
		{
			ps_src.push_back((char*)t.c_str());
		}

		std::string log;

		auto program = CompileShader(vs_src, ps_src, log);

		result.ID = program;
		result.ErrorMessage = log;

		return program > 0;
	}
}

