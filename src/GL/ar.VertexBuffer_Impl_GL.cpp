
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

	bool VertexBuffer_Impl_GL::Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount, bool isDynamic)
	{
		auto m = (Manager_Impl_GL*)manager;

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, vertexSize * vertexCount, nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		this->manager = manager;
		this->vertexSize = vertexSize;
		this->vertexCount = vertexCount;
		this->isDynamic = isDynamic;

		if (this->isDynamic)
		{
			this->dynamicBuffer.resize(vertexSize * vertexCount);
		}

		return true;

	}

	bool VertexBuffer_Impl_GL::Write(const void* data, int32_t size)
	{
		if (size != vertexSize * vertexCount) return false;

		auto m = (Manager_Impl_GL*)manager;

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, vertexSize * vertexCount, data, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		ringOffset = 0;
		vertexOffset = ringOffset;

		return true;
	}

	void* VertexBuffer_Impl_GL::LockRingBuffer(int32_t count)
	{
		if (count > vertexCount) return nullptr;
		if (!isDynamic) return nullptr;

		// Todo : correct it.
		if(true)
		//if (ringOffset + count >= vertexCount)
		{
			ringOffset = 0;
		}

		auto p = &(dynamicBuffer[ringOffset * vertexSize]);

		ringLocked = count;
		vertexOffset = ringOffset;

		return p;
	}

	void VertexBuffer_Impl_GL::Unlock()
	{
		auto m = (Manager_Impl_GL*)manager;

		GLCheckError();

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		GLCheckError();

		glBufferSubData(GL_ARRAY_BUFFER, ringOffset * vertexSize, ringLocked * vertexSize, dynamicBuffer.data());
		GLCheckError();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		GLCheckError();

		ringOffset += ringLocked;
	}
}
