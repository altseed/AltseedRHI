#include "ar.PNGHelper_Impl_Win.h"



namespace ar
{
	PNGHelper_Impl_Win::PNGHelper_Impl_Win()
	{
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		tempBuffer1.resize(2048 * 2048 * 4);
	}

	PNGHelper_Impl_Win::~PNGHelper_Impl_Win()
	{
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	bool PNGHelper_Impl_Win::Save(const char16_t* path, int32_t width, int32_t height, const void* data)
	{
			// Create bitmap data
			Gdiplus::Bitmap bmp(width, height);

			auto p = (Color*)data;
			for (int32_t y = 0; y < height; y++)
			{
				for (int32_t x = 0; x < width; x++)
				{
					auto src = p[x + width * y];
					Gdiplus::Color dst(src.A, src.R, src.G, src.B);
					bmp.SetPixel(x, y, dst);
				}
			}

			// Save bitmap
			CLSID id;
			UINT encoderNum = 0;
			UINT encoderSize = 0;
			Gdiplus::GetImageEncodersSize(&encoderNum, &encoderSize);
			if (encoderSize == 0)
			{
				return false;
			}

			auto imageCodecInfo = (Gdiplus::ImageCodecInfo*) malloc(encoderSize);
			Gdiplus::GetImageEncoders(encoderNum, encoderSize, imageCodecInfo);

			for (UINT i = 0; i < encoderNum; i++)
			{
				if (wcscmp(imageCodecInfo[i].MimeType, L"image/png") == 0)
				{
					id = imageCodecInfo[i].Clsid;
					free(imageCodecInfo);
					imageCodecInfo = nullptr;
					break;
				}
			}

			if (imageCodecInfo != nullptr)
			{
				free(imageCodecInfo);
				return false;
			}

			bmp.Save((const wchar_t*)path, &id);
		

		return true;
	}

	bool PNGHelper_Impl_Win::Read(PNGLoadFunc readFunc, void* userData, const void* src, int32_t src_size)
	{
		auto global = GlobalAlloc(GMEM_MOVEABLE, src_size);
		auto buf = GlobalLock(global);
		CopyMemory(buf, src, src_size);
		GlobalUnlock(global);
		LPSTREAM stream = NULL;
		CreateStreamOnHGlobal(global, false, &stream);
		Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromStream(stream);
		SafeRelease(stream);
		GlobalFree(global);


		if (bmp != NULL && bmp->GetLastStatus() == Gdiplus::Ok)
		{
			auto width = bmp->GetWidth();
			auto height = bmp->GetHeight();
			tempBuffer1.resize(width * height * 4);
			
			for (auto y = 0; y < height; y++)
			{
				for (auto x = 0; x < width; x++)
				{
					Gdiplus::Color  color;
					bmp->GetPixel(x, y, &color);

					tempBuffer1[(x + y * width) * 4 + 0] = color.GetR();
					tempBuffer1[(x + y * width) * 4 + 1] = color.GetG();
					tempBuffer1[(x + y * width) * 4 + 2] = color.GetB();
					tempBuffer1[(x + y * width) * 4 + 3] = color.GetA();
				}
			}
			
			readFunc(tempBuffer1.data(), width, height, userData);

			SafeDelete(bmp);
			return true;
		}
		else
		{
			SafeDelete(bmp);
			return false;
		}

		SafeDelete(bmp);
		return true;
	}
}