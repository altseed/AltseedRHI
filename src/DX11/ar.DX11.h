
#pragma once

#if defined(_WIN32)

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib" )
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include <D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib" )

#endif

#include "../ar.BaseInternal.h"

namespace ar
{

static TextureFormat GetTextureFormat(DXGI_FORMAT format)
{
	if (format == DXGI_FORMAT_R8G8B8A8_UNORM) return TextureFormat::R8G8B8A8_UNORM;
	if (format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB) return TextureFormat::R8G8B8A8_UNORM_SRGB;
	if (format == DXGI_FORMAT_R16G16B16A16_FLOAT) return TextureFormat::R16G16B16A16_FLOAT;
	if (format == DXGI_FORMAT_R32G32B32A32_FLOAT) return TextureFormat::R32G32B32A32_FLOAT;
	if (format == DXGI_FORMAT_R16G16_FLOAT) return TextureFormat::R16G16_FLOAT;
	if (format == DXGI_FORMAT_R8_UNORM) return TextureFormat::R8_UNORM;
	if (format == DXGI_FORMAT_BC1_UNORM) return TextureFormat::BC1;
	if (format == DXGI_FORMAT_BC1_UNORM_SRGB) return TextureFormat::BC1_SRGB;
	if (format == DXGI_FORMAT_BC2_UNORM) return TextureFormat::BC2;
	if (format == DXGI_FORMAT_BC2_UNORM_SRGB) return TextureFormat::BC2_SRGB;
	if (format == DXGI_FORMAT_BC3_UNORM) return TextureFormat::BC3;
	if (format == DXGI_FORMAT_BC3_UNORM_SRGB) return TextureFormat::BC3_SRGB;

	assert(0);
}

}