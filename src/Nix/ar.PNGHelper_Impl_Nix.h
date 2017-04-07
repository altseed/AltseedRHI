
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

		bool Read(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size) override;
	};
}