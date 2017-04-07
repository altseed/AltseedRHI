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