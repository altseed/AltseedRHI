
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
	public:
		Shader_Impl_GL();

		virtual ~Shader_Impl_GL();

		bool Initialize(Manager* manager, const void* vs, int32_t vs_size, const void* ps, int32_t ps_size, const std::vector <VertexLayout>& layout) override;

		GLuint GetShader() const { return program; }

		const std::vector<Layout>& GetLayouts() { return layouts; }
	};

}
