
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

	class IndexBuffer_Impl_GL
		: public IndexBuffer
	{
	private:
		Manager*		manager = nullptr;
        GLuint			buffer = 0;
		
	public:
		IndexBuffer_Impl_GL();

		virtual ~IndexBuffer_Impl_GL();

		bool Initialize(Manager* manager, int32_t indexCount, bool is32bit) override;

		bool Write(const void* data, int32_t size) override;

		GLuint GetBuffer() const { return buffer; }
	};

}
