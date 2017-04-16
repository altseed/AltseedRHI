
#pragma once

#include "../ar.BaseInternal.h"

namespace ar
{
	class PNGHelper_Impl_Nix
		: public PNGHelper
	{
	private:
		std::vector<uint8_t>	tempBuffer1;

	public:

		PNGHelper_Impl_Nix();
		virtual ~PNGHelper_Impl_Nix();

		bool Save(const char16_t* path, int32_t width, int32_t height, const void* data) override;
		bool Read(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size) override;
	};
}