
#include "ar.VertexBuffer_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

namespace ar
{
	VertexBuffer_Impl_GL::VertexBuffer_Impl_GL()
	{

	}

	VertexBuffer_Impl_GL::~VertexBuffer_Impl_GL()
	{
		glDeleteBuffers(1, &buffer);
	}

	bool VertexBuffer_Impl_GL::Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount)
	{
		auto m = (Manager_Impl_GL*)manager;

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, vertexSize * vertexCount, nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->manager = manager;
		this->vertexSize = vertexSize;
		this->vertexCount = vertexCount;

		return true;

	}

	bool VertexBuffer_Impl_GL::Write(const void* data, int32_t size)
	{
		if (size != vertexSize * vertexCount) return false;

		auto m = (Manager_Impl_GL*)manager;

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, vertexSize * vertexCount, data, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	}
}
