#include "ar.PNGHelper_Impl_Nix.h"

#define Z_SOLO
#include <png.h>

#include <codecvt>
#include <locale>
#include <string.h>

namespace ar
{
	// http://hasenpfote36.blogspot.jp/2016/09/stdcodecvt.html
	static constexpr std::codecvt_mode mode = std::codecvt_mode::little_endian;

	static std::string utf16_to_utf8(const std::u16string& s)
	{
#if defined(_MSC_VER) && (_MSC_VER <= 1900)
		std::wstring_convert<std::codecvt_utf8_utf16<std::uint16_t, 0x10ffff, mode>, std::uint16_t> conv;
		auto p = reinterpret_cast<const std::uint16_t*>(s.c_str());
		return conv.to_bytes(p, p + s.length());
#else
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, mode>, char16_t> conv;
		return conv.to_bytes(s);
#endif
	}

	static void PngReadData(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		auto d = (uint8_t**)png_get_io_ptr(png_ptr);

		memcpy(data, *d, length);
		(*d) += length;
	}

	PNGHelper_Impl_Nix::PNGHelper_Impl_Nix()
	{
		tempBuffer1.resize(2048 * 2048 * 4);
	}

	PNGHelper_Impl_Nix::~PNGHelper_Impl_Nix()
	{

	}

	bool PNGHelper_Impl_Nix::Save(const char16_t* path, int32_t width, int32_t height, const void* data)
	{
		/* 構造体確保 */
#if _WIN32
		FILE *fp = _wfopen((const wchar_t*)path, L"wb");
#else
		FILE *fp = fopen(utf16_to_utf8(path).c_str(), "wb");
#endif

		if (fp == nullptr) return false;

		png_structp pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		png_infop ip = png_create_info_struct(pp);

		/* 書き込み準備 */
		png_init_io(pp, fp);
		png_set_IHDR(pp, ip, width, height,
			8, /* 8bit以外にするなら変える */
			PNG_COLOR_TYPE_RGBA, /* RGBA以外にするなら変える */
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

		/* ピクセル領域確保 */
		std::vector<png_byte>  raw1D(height * png_get_rowbytes(pp, ip));
		std::vector<png_bytep> raw2D(height * sizeof(png_bytep));
		for (int32_t i = 0; i < height; i++)
		{
			raw2D[i] = &raw1D[i*png_get_rowbytes(pp, ip)];
		}

		memcpy((void*)raw1D.data(), data, width * height * 4);

		/* 書き込み */
		png_write_info(pp, ip);
		png_write_image(pp, raw2D.data());
		png_write_end(pp, ip);

		/* 解放 */
		png_destroy_write_struct(&pp, &ip);
		fclose(fp);
		return true;
	}

	bool PNGHelper_Impl_Nix::Read(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size)
	{
		uint8_t* data_ = (uint8_t*)src;

		/* pngアクセス構造体を作成 */
		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		/* リードコールバック関数指定 */
		png_set_read_fn(png, &data_, &PngReadData);

		/* png画像情報構造体を作成 */
		png_infop png_info = png_create_info_struct(png);

		/* エラーハンドリング */
		if (setjmp(png_jmpbuf(png)))
		{
			png_destroy_read_struct(&png, &png_info, NULL);
			return false;
		}

		/* IHDRチャンク情報を取得 */
		png_read_info(png, png_info);

		/* インターレース */
		auto number_of_passes = png_set_interlace_handling(png);
		if (number_of_passes == 0)
		{
			number_of_passes = 1;
		}

		/* RGBA8888フォーマットに変換する */
		png_byte bit_depth = png_get_bit_depth(png, png_info);
		if (bit_depth < 8)
		{
			png_set_packing(png);
		}
		else if (bit_depth == 16)
		{
			png_set_strip_16(png);
		}

		uint32_t pixelBytes = 4;
		png_byte color_type = png_get_color_type(png, png_info);
		switch (color_type)
		{
		case PNG_COLOR_TYPE_PALETTE:
		{
			png_set_palette_to_rgb(png);

			png_bytep trans_alpha = NULL;
			int num_trans = 0;
			png_color_16p trans_color = NULL;

			png_get_tRNS(png, png_info, &trans_alpha, &num_trans, &trans_color);
			if (trans_alpha != NULL)
			{
				pixelBytes = 4;
			}
			else
			{
				pixelBytes = 3;
			}
		}
		break;
		case PNG_COLOR_TYPE_GRAY:
			png_set_expand_gray_1_2_4_to_8(png);
			png_set_gray_to_rgb(png);
			pixelBytes = 3;
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			png_set_gray_to_rgb(png);
			pixelBytes = 4;
			break;
		case PNG_COLOR_TYPE_RGB:
			pixelBytes = 3;
			break;
		case PNG_COLOR_TYPE_RGBA:
			break;
		}

		png_uint_32 width = png_get_image_width(png, png_info);
		png_uint_32 height = png_get_image_height(png, png_info);
		uint8_t* image = new uint8_t[width * height * pixelBytes];
		uint32_t pitch = width * pixelBytes;

		// 読み込み
		bool rev = false; // TODO: Y方向を反転させるフラグをとる必要がある。
		if (rev)
		{
			png_bytepp rp;
			for (auto pass = 0; pass < number_of_passes; pass++)
			{
				for (uint32_t i = 0; i < height; i++)
				{
					png_read_row(png, &image[(height - 1 - i) * pitch], NULL);
				}
			}
		}
		else
		{
			for (auto pass = 0; pass < number_of_passes; pass++)
			{
				for (uint32_t i = 0; i < height; i++)
				{
					png_read_row(png, &image[i * pitch], NULL);
				}
			}
		}

		auto imagewidth = width;
		auto imageheight = height;
		tempBuffer1.resize(imagewidth * imageheight * 4);
		auto imagedst_ = tempBuffer1.data();

		if (pixelBytes == 4)
		{
			memcpy(imagedst_, image, imagewidth * imageheight * 4);
		}
		else
		{
			for (int32_t y = 0; y < imageheight; y++)
			{
				for (int32_t x = 0; x < imagewidth; x++)
				{
					auto src = (x + y * imagewidth) * 3;
					auto dst = (x + y * imagewidth) * 4;
					imagedst_[dst + 0] = image[src + 0];
					imagedst_[dst + 1] = image[src + 1];
					imagedst_[dst + 2] = image[src + 2];
					imagedst_[dst + 3] = 255;
				}
			}
		}

		readFunc(tempBuffer1.data(), imagewidth, imageheight, userData);

		SafeDeleteArray(image);
		png_destroy_read_struct(&png, &png_info, NULL);

		return true;
	}
}