
#include "ar.ImageHelper.h"

#include "ar.BaseInternal.h"

#if defined(_PSVITA)
#include "PSVITA/ar.PNGHelper_Impl_PSVITA.h"
#elif defined(_WIN32)
#include "Win/ar.PNGHelper_Impl_Win.h"
#else
#include "Nix/ar.PNGHelper_Impl_Nix.h"
#endif

namespace ar
{

// Singleton
static PNGHelper*	pngHelper = nullptr;

int32_t	ImageHelper::reference = 0;

void ImageHelper::Initizlize()
{
	if (reference == 0)
	{
#if defined(_PSVITA)
		pngHelper = new PNGHelper_Impl_PSVITA();
#elif defined(_WIN32)
		pngHelper = new PNGHelper_Impl_Win();
#else
		pngHelper = new PNGHelper_Impl_Nix();
#endif
	}

	reference++;
}

void ImageHelper::Terminate()
{
	reference--;

	if (reference == 0)
	{
		SafeDelete(pngHelper);
	}
}


bool ImageHelper::SavePNG(const char16_t* path, int32_t width, int32_t height, const void* data)
{
	if (pngHelper != nullptr)
	{
		return pngHelper->Save(path, width, height, data);
	}

	return false;
}

bool ImageHelper::LoadPNG(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size)
{
	if (pngHelper != nullptr)
	{
		return pngHelper->Read(readFunc, userData, src, src_size);
	}

	return false;
}

int32_t ImageHelper::GetPitch(TextureFormat format)
{
	if (format == TextureFormat::R8G8B8A8_UNORM) return 4;
	if (format == TextureFormat::R16G16B16A16_FLOAT) return 2 * 4;
	if (format == TextureFormat::R32G32B32A32_FLOAT) return 4 * 4;
	if (format == TextureFormat::R8G8B8A8_UNORM_SRGB) return 4;
	if (format == TextureFormat::R16G16_FLOAT) return 2 * 2;
	if (format == TextureFormat::R8_UNORM) return 1;

	// 1ピクセル単位で返せない
	if (format == TextureFormat::BC1) return 0;
	if (format == TextureFormat::BC1_SRGB) return 0;
	if (format == TextureFormat::BC2) return 0;
	if (format == TextureFormat::BC2_SRGB) return 0;
	if (format == TextureFormat::BC3) return 0;
	if (format == TextureFormat::BC3_SRGB) return 0;

	return 0;
}

int32_t ImageHelper::GetVRAMSize(TextureFormat format, int32_t width, int32_t height)
{
	auto pitch = GetPitch(format);

	if (pitch == 0)
	{
		if (format == TextureFormat::BC1 ||
			format == TextureFormat::BC1_SRGB) return width * height * 4 / 6;

		if (format == TextureFormat::BC2 ||
			format == TextureFormat::BC2_SRGB) return width * height * 4 / 4;

		if (format == TextureFormat::BC3 ||
			format == TextureFormat::BC3_SRGB) return width * height * 4 / 4;
	}

	return pitch * width * height;
}

int32_t ImageHelper::GetMipmapCount(int32_t width, int32_t height)
{
	auto ret = 1;
	while (width != 1 || height != 1)
	{
		if (width > 1) width = width >> 1;
		if (height > 1) height = height >> 1;
		ret++;
	}

	return ret;
}

void ImageHelper::GetMipmapSize(int mipmap, int32_t& width, int32_t& height)
{
	for (auto i = 0; i < mipmap; i++)
	{
		if (width > 1) width = width >> 1;
		if (height > 1) height = height >> 1;
	}
}


bool ImageHelper::IsPNG(const void* data, int32_t size)
{
	if (size < 4) return false;

	auto d = (uint8_t*)data;

	if (d[0] != 0x89) return false;
	if (d[1] != 'P') return false;
	if (d[2] != 'N') return false;
	if (d[3] != 'G') return false;

	return true;
}

bool ImageHelper::IsDDS(const void* data, int32_t size)
{
	if (size < 4) return false;

	auto d = (uint8_t*)data;

	if (d[0] != 'D') return false;
	if (d[1] != 'D') return false;
	if (d[2] != 'S') return false;

	return true;
}

std::string ImageHelper::GetFormatName(TextureFormat format)
{
	if (format == TextureFormat::R8_UNORM) return std::string("R8_UNORM");
	if (format == TextureFormat::R16G16B16A16_FLOAT) return std::string("R16G16B16A16_FLOAT");
	if (format == TextureFormat::R32G32B32A32_FLOAT) return std::string("R32G32B32A32_FLOAT");
	if (format == TextureFormat::R8G8B8A8_UNORM) return std::string("R8G8B8A8_UNORM");
	if (format == TextureFormat::R8G8B8A8_UNORM_SRGB) return std::string("R8G8B8A8_UNORM_SRGB");
	if (format == TextureFormat::R16G16_FLOAT) return std::string("R16G16_FLOAT");
	return std::string("Unknown");
}

}