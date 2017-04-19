
#include "ar.IndexBuffer_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

namespace ar
{
	IndexBuffer_Impl_GL::IndexBuffer_Impl_GL()
	{

	}

	IndexBuffer_Impl_GL::~IndexBuffer_Impl_GL()
	{
        glDeleteBuffers(1, &buffer);
	}

	bool IndexBuffer_Impl_GL::Initialize(Manager* manager, int32_t indexCount, bool is32bit)
	{
		auto m = (Manager_Impl_GL*)manager;
        
        glGenBuffers(1, &buffer);
        
		this->manager = manager;
        this->indexCount = indexCount;
		this->is32bit = is32bit;

        return glGetError() == GL_NO_ERROR;
	}

	bool IndexBuffer_Impl_GL::Write(const void* data, int32_t size)
	{
		int32_t isize = is32bit ? 4 : 2;

		if (size != indexCount * isize) return false;

		auto m = (Manager_Impl_GL*)manager;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * isize, data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}
}
