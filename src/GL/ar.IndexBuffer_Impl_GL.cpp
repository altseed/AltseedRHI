
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

	bool IndexBuffer_Impl_GL::Initialize(Manager* manager, int32_t indexCount)
	{
		auto m = (Manager_Impl_GL*)manager;
        
        glGenBuffers(1, &buffer);
        
		this->manager = manager;
        this->indexCount = indexCount;
        
        return glGetError() == GL_NO_ERROR;
	}

	bool IndexBuffer_Impl_GL::Write(const void* data, int32_t size)
	{
		if (size != indexCount * sizeof(int16_t)) return false;

		auto m = (Manager_Impl_GL*)manager;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int16_t), data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}
}
