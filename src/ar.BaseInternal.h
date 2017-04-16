
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

		virtual bool Save(const char16_t* path, int32_t width, int32_t height, const void* data) { return false; }

		virtual bool Read(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size) { return false; }
	};
}