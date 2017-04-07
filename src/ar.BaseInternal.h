
#pragma once

#include "ar.Base.h"

/**
	@brief	Common header used only in this project.
*/

namespace ar
{
	class PNGHelper
	{
	public:
		PNGHelper() {}
		virtual ~PNGHelper() {}

		virtual bool Read(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size) = 0;
	};
}