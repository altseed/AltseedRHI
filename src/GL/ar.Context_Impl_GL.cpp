
#include "ar.Context_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"
#include "ar.VertexBuffer_Impl_GL.h"
#include "ar.IndexBuffer_Impl_GL.h"
#include "ar.Shader_Impl_GL.h"

namespace ar
{
	Context_Impl_GL::Context_Impl_GL()
	{

	}

	Context_Impl_GL::~Context_Impl_GL()
	{
		//SafeRelease(context);
	}

	bool Context_Impl_GL::Initialize(Manager* manager)
	{
		/*
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;

		this->context = m->GetContext();
		SafeAddRef(this->context);

		return true;

	End:;
		SafeRelease(this->context);
		*/

		return false;
	}


	void Context_Impl_GL::Draw(const DrawParameter& param)
	{
		auto vb = (VertexBuffer_Impl_GL*)param.VertexBufferPtr;
		auto ib = (IndexBuffer_Impl_GL*)param.IndexBufferPtr;
		auto shader = (Shader_Impl_GL*)param.ShaderPtr;

		glBindBuffer(GL_ARRAY_BUFFER, vb->GetBuffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->GetBuffer());

		glUseProgram(shader->GetShader());

		const void* vertices = NULL;

		for (auto& layout : shader->GetLayouts())
		{
			if (layout.attribute == 0) continue;

			glEnableVertexAttribArray(layout.attribute);
			glVertexAttribPointer(layout.attribute, layout.count, layout.type, layout.normalized, vb->GetVertexSize(), (uint8_t*)vertices + layout.offset);
		}

		glDrawElements(GL_TRIANGLES, ib->GetIndexCount(), GL_UNSIGNED_SHORT, NULL);

		for (auto& layout : shader->GetLayouts())
		{
			if (layout.attribute >= 0)
			{
				glDisableVertexAttribArray(layout.attribute);
			}
		}

		glUseProgram(0);
	}
}
