
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{
	class Context_Impl_GL
		: public Context
	{
	private:
		//ID3D11DeviceContext*	context = nullptr;

	public:
		Context_Impl_GL();

		virtual ~Context_Impl_GL();

		bool Initialize(Manager* manager);

		void Draw(const DrawParameter& param) override;
	};

}
