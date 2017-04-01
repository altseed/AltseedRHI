
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{
	class Context_Impl_DX11
		: public Context
	{
	private:
		ID3D11DeviceContext*	context = nullptr;

	public:
		Context_Impl_DX11();

		virtual ~Context_Impl_DX11();

		bool Initialize(Manager* manager);

		void Draw(const DrawParameter& param) override;
	};

}