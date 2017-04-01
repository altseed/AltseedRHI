
#include "ar.Context_Impl_DX11.h"

#include "ar.Manager_Impl_DX11.h"
#include "ar.VertexBuffer_Impl_DX11.h"
#include "ar.IndexBuffer_Impl_DX11.h"
#include "ar.Shader_Impl_DX11.h"

namespace ar
{
	Context_Impl_DX11::Context_Impl_DX11()
	{

	}

	Context_Impl_DX11::~Context_Impl_DX11()
	{
		SafeRelease(context);
	}

	bool Context_Impl_DX11::Initialize(Manager* manager)
	{
		auto m = (Manager_Impl_DX11*)manager;
		HRESULT hr;

		this->context = m->GetContext();
		SafeAddRef(this->context);

		return true;

	End:;
		SafeRelease(this->context);

		return false;
	}


	void Context_Impl_DX11::Draw(const DrawParameter& param)
	{
		auto vb = (VertexBuffer_Impl_DX11*)param.VertexBufferPtr;
		auto ib = (IndexBuffer_Impl_DX11*)param.IndexBufferPtr;
		auto shader = (Shader_Impl_DX11*)param.ShaderPtr;

		// Set vertex buffer
		auto vbuf = vb->GetBuffer();
		uint32_t vertexSize = vb->GetVertexSize();
		uint32_t voffset = 0;
		context->IASetVertexBuffers(0, 1, &vbuf, &vertexSize, &voffset);
		
		// Set index buffer
		context->IASetIndexBuffer(ib->GetBuffer(), DXGI_FORMAT_R16_UINT, 0);

		// Set shader
		context->VSSetShader(shader->GetVertexShader(), NULL, 0);
		context->PSSetShader(shader->GetPixelShader(), NULL, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetInputLayout(shader->GetInputLayout());

		// Draw
		int32_t vertexBufferOffset = 0;
		context->DrawIndexed(
			ib->GetIndexCount(),
			0,
			vertexBufferOffset);
	}
}