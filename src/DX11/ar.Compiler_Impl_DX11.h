
#pragma once

#include "ar.DX11.h"

#include "../ar.Base.h"

namespace ar
{

	class Compiler_Impl_DX11
		: public Compiler
	{
	private:
		Manager*		manager = nullptr;

	public:
		Compiler_Impl_DX11();
		virtual ~Compiler_Impl_DX11();

		bool Compile(ShaderCompilerResult& result, ShaderCompilerParameter& param) override;
	};

}