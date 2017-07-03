
#include "common.h"

#include <iostream>
#include <fstream>


#if defined(_PSVITA)
#include "PSVITA.h"

#elif defined(_PS4)

#elif defined(_SWITCH)

#elif defined(_XBOXONE)

#elif defined(_WIN32)

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"x64/Debug/glfw3.lib")
#pragma comment(lib,"x64/Debug/libglew32d.lib")
#pragma comment(lib,"x64/Debug/AltseedPlatform.lib")
#else
#pragma comment(lib,"x64/Release/glfw3.lib")
#pragma comment(lib,"x64/Debug/libglew32.lib")
#pragma comment(lib,"x64/Release/AltseedPlatform.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib,"x86/Debug/glfw3.lib")
#pragma comment(lib,"x86/Debug/libglew32d.lib")
#pragma comment(lib,"x86/Debug/AltseedPlatform.lib")
#else
#pragma comment(lib,"x86/Release/glfw3.lib")
#pragma comment(lib,"x86/Debug/libglew32.lib")
#pragma comment(lib,"x86/Release/AltseedPlatform.lib")
#endif

#endif

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib, "gdiplus.lib")

#else

#endif

bool LoadShaderFile(std::vector<uint8_t>& dst, const char* path, ar::GraphicsDeviceType device)
{
	std::string p;
	bool isScriptMode = false;

#if _PSVITA
	p = std::string(ShaderRootPath) + std::string(path);
#elif defined(_WIN32)
	if (device == ar::GraphicsDeviceType::OpenGL)
	{
		p = std::string("GLSL_3/") + std::string(path);
		isScriptMode = true;
	}
	else
	{
		p = std::string("HLSL/") + std::string(path);
	}
#else
    if (device == ar::GraphicsDeviceType::OpenGL)
    {
        p = std::string("GLSL_3/") + std::string(path);
        isScriptMode = true;
    }
#endif

	auto fp = fopen(p.c_str(), "rb");
	if (fp == nullptr) return false;
	fseek(fp, 0, SEEK_END);
	auto size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	dst.resize(size);

	fread(dst.data(), 1, size, fp);

	fclose(fp);

	if (isScriptMode)
	{
		dst.push_back(0);
		dst.push_back(0);
	}
	return true;
}

bool LoadFile(std::vector<uint8_t>& dst, const char* path)
{
#if _PSVITA
	auto p = std::string(RootPath) + std::string(path);
#else
	auto p = std::string("") + std::string(path);
#endif

	auto fp = fopen(p.c_str(), "rb");
	if (fp == nullptr) return false;
	fseek(fp, 0, SEEK_END);
	auto size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	dst.resize(size);

	fread(dst.data(), 1, size, fp);

	fclose(fp);

	return true;
}
