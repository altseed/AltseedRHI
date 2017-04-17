
#pragma once

#include "ar.GL.h"

#include "../ar.Base.h"

namespace ar
{

	class Compiler_Impl_GL
		: public Compiler
	{
	private:
		Manager*		manager = nullptr;

	public:
		Compiler_Impl_GL();
		virtual ~Compiler_Impl_GL();

		bool Compile(ShaderCompilerResult& result, ShaderCompilerParameter& param) override;
	};

}