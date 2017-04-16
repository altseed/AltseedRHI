
#pragma once

#include "ar.Base.h"

namespace ar
{
class ImageHelper
{
	static int32_t	reference;
public:

	static void Initizlize();

	static void Terminate();

	static bool SavePNG(const char16_t* path, int32_t width, int32_t height, const void* data);
	static bool LoadPNG(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size);

	/**
	@brief	1ピクセルのサイズを取得する。
	@param	format	フォーマット
	@return	サイズ
	*/
	static int32_t GetPitch(TextureFormat format);

	/**
	@brief	画像が消費する推定VRAMサイズを取得する。
	*/
	static int32_t GetVRAMSize(TextureFormat format, int32_t width, int32_t height);

	static int32_t GetMipmapCount(int32_t width, int32_t height);
	static void GetMipmapSize(int mipmap, int32_t& width, int32_t& height);

	static bool IsPNG(const void* data, int32_t size);
	static bool IsDDS(const void* data, int32_t size);

	static std::string GetFormatName(TextureFormat format);
};

}