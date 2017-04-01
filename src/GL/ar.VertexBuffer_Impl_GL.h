
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

	class VertexBuffer_Impl_GL
		: public VertexBuffer
	{
	private:
		Manager*		manager = nullptr;
		GLuint          buffer = 0;

	public:
		VertexBuffer_Impl_GL();

		virtual ~VertexBuffer_Impl_GL();

		bool Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount) override;

		bool Write(const void* data, int32_t size) override;

		GLuint GetBuffer() const { return buffer; }
	};

}
