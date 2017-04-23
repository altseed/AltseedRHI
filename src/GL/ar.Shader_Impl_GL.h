
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

	class Shader_Impl_GL
		: public Shader
	{
	public:
		struct Layout
		{
			GLint		attribute;
			GLenum		type;
			uint16_t	count;
			uint16_t	offset;
			bool		normalized;
		};

	private:
		GLuint				program = 0;
		std::vector<Layout>	layouts;

		void Reflect(
			std::map<std::string, ConstantLayout>& dst_constant,
			int32_t& constantSize,
			std::map<std::string, TextureLayout>& dst_texture,
			int32_t& textureCount,
			GLuint program);

	public:
		Shader_Impl_GL();

		virtual ~Shader_Impl_GL();

		bool Initialize(Manager* manager, const ShaderCompilerResult& compilerResult, const std::vector <VertexLayout>& layout, bool is32bit) override;

		GLuint GetShader() const { return program; }

		const std::vector<Layout>& GetLayouts() { return layouts; }
	};

}
