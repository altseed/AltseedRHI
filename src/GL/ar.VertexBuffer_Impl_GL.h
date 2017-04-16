
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

		int32_t					ringOffset = 0;
		int32_t					ringLocked = 0;

		std::vector<uint8_t>	dynamicBuffer;

	public:
		VertexBuffer_Impl_GL();

		virtual ~VertexBuffer_Impl_GL();

		bool Initialize(Manager* manager, int32_t vertexSize, int32_t vertexCount, bool isDynamic) override;

		bool Write(const void* data, int32_t size) override;

		void* LockRingBuffer(int32_t count) override;

		void Unlock() override;

		GLuint GetBuffer() const { return buffer; }
	};

}
