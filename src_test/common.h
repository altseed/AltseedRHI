
#pragma once

#include <AltseedPlatform.h>
#include <AltseedRHI.h>

struct SimpleVertex
{
	float x, y, z;
	float u, v;
	uint8_t r, g, b, a;

	SimpleVertex()
	{
		memset(this, 0, sizeof(SimpleVertex));
	}
};

bool LoadShaderFile(std::vector<uint8_t>& dst, const char* path, ar::GraphicsDeviceType device = ar::GraphicsDeviceType::Default);

bool LoadFile(std::vector<uint8_t>& dst, const char* path);