
#pragma once

#include "../ar.BaseInternal.h"

#define WINVER          0x0501
#define _WIN32_WINNT    0x0501
#include <windows.h>
#include <gdiplus.h>

namespace ar
{
	class PNGHelper_Impl_Win
		: public PNGHelper
	{
	private:
		std::vector<uint8_t>	tempBuffer1;

		Gdiplus::GdiplusStartupInput		gdiplusStartupInput;
		ULONG_PTR						gdiplusToken;

	public:

		PNGHelper_Impl_Win();
		virtual ~PNGHelper_Impl_Win();

		bool Save(const char16_t* path, int32_t width, int32_t height, const void* data) override;

		bool Read(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size) override;
	};
}