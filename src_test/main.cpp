
#include <stdio.h>

#include <ap.Window.h>
#include <ap.Joystick.h>

#include <ar.Base.h>
#include <ar.ImageHelper.h>


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
#pragma comment(lib,"x64/Debug/AltseedPlatform_Win.lib")
#else
#pragma comment(lib,"x64/Release/glfw3.lib")
#pragma comment(lib,"x64/Release/AltseedPlatform_Win.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib,"x86/Debug/glfw3.lib")
#pragma comment(lib,"x86/Debug/AltseedPlatform_Win.lib")
#else
#pragma comment(lib,"x86/Release/glfw3.lib")
#pragma comment(lib,"x86/Release/AltseedPlatform_Win.lib")
#endif

#endif

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib, "gdiplus.lib")

#else

#endif


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

bool LoadFile(std::vector<uint8_t>& dst, const char* path)
{
	auto fp = fopen(path, "rb");
	if (fp == nullptr) return false;
	fseek(fp, 0, SEEK_END);
	auto size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	dst.resize(size);

	fread(dst.data(), 1, size, fp);

	fclose(fp);

	return true;
}



struct LoadTextureInfo
{
	ar::Manager* manager = nullptr;
	ar::Texture2D* texture = nullptr;
};

void loadTexture(const uint8_t* data, int32_t width, int32_t height, void* userData)
{
	auto p = (LoadTextureInfo*)userData;

	p->texture = ar::Texture2D::Create();
	p->texture->Initialize(p->manager, width, height, ar::TextureFormat::R8G8B8A8_UNORM, (void*)data, false);
}

int main()
{
	auto window = ap::Window::Create();
	
	ap::WindowInitializationParameter wparam;
	wparam.WindowWidth = 640;
	wparam.WindowHeight = 480;
	wparam.GraphicsDevice = ap::GraphicsDeviceType::DirectX11;

	window->Initialize(wparam);

	auto joystick = ap::Joystick::Create(window);

	auto manager = ar::Manager::Create();
	ar::ManagerInitializationParameter mparam;

	mparam.Handles[0] = nullptr;
	mparam.WindowWidth = 640;
	mparam.WindowHeight = 480;
	manager->Initialize(mparam);
	
	auto vertexBuffer = ar::VertexBuffer::Create();
	vertexBuffer->Initialize(manager, sizeof(SimpleVertex), 4);

	std::array<SimpleVertex, 4> vbuffer;
	vbuffer[0].x = -1.0f;
	vbuffer[0].y = 1.0f;
	vbuffer[1].x = 1.0f;
	vbuffer[1].y = 1.0f;
	vbuffer[2].x = 1.0f;
	vbuffer[2].y = -1.0f;
	vbuffer[3].x = -1.0f;
	vbuffer[3].y = -1.0f;

	vbuffer[0].u = 0;
	vbuffer[0].v = 0;
	vbuffer[1].u = 1;
	vbuffer[1].v = 0;
	vbuffer[2].u = 1;
	vbuffer[2].v = 1;
	vbuffer[3].u = 0;
	vbuffer[3].v = 1;

	vbuffer[3].r = 255;
	vbuffer[3].g = 255;
	vbuffer[3].b = 255;
	vbuffer[3].a = 255;

	vertexBuffer->Write(vbuffer.data(), sizeof(SimpleVertex) * 4);

	auto indexBuffer = ar::IndexBuffer::Create();
	indexBuffer->Initialize(manager, 6);

	std::array<int16_t, 6> ibuffer;
	ibuffer[0] = 0;
	ibuffer[1] = 1;
	ibuffer[2] = 2;
	ibuffer[3] = 0;
	ibuffer[4] = 2;
	ibuffer[5] = 3;

	indexBuffer->Write(ibuffer.data(), sizeof(int16_t) * 6);

	auto shader = ar::Shader::Create();

	std::vector <ar::VertexLayout> vertexLayouts;
	vertexLayouts.resize(3);
	vertexLayouts[0].Name = "Pos";
	vertexLayouts[0].LayoutFormat = ar::VertexLayoutFormat::R32G32B32_FLOAT;
	vertexLayouts[1].Name = "UV";
	vertexLayouts[1].LayoutFormat = ar::VertexLayoutFormat::R32G32_FLOAT;
	vertexLayouts[2].Name = "Color";
	vertexLayouts[2].LayoutFormat = ar::VertexLayoutFormat::R8G8B8A8_UNORM;

	std::string rootPath = "HLSL/";
#if defined(_WIN32)
#else
	rootPath = RootPath;
#endif

	std::vector<uint8_t> shader_vs;
	std::vector<uint8_t> shader_ps;

	LoadFile(shader_vs, (rootPath + "texture_VS.dat").c_str());
	LoadFile(shader_ps, (rootPath + "texture_PS.dat").c_str());

	shader->Initialize(manager, shader_vs.data(), shader_vs.size(), shader_ps.data(), shader_ps.size(), vertexLayouts);


	auto context = ar::Context::Create();
	context->Initialize(manager);


	std::vector<uint8_t> textureBuf;
	LoadFile(textureBuf, "Data/Texture01.png");
	LoadTextureInfo ltInfo;
	ltInfo.manager = manager;
	ar::ImageHelper::LoadPNG(loadTexture, &ltInfo, textureBuf.data(), textureBuf.size());

	ar::Texture2D* texture = ltInfo.texture;

	while (window->DoEvent())
	{
		manager->BeginRendering();

		ar::SceneParameter sparam;

		manager->BeginScene(sparam);

		ar::DrawParameter dparam;
		dparam.VertexBufferPtr = vertexBuffer;
		dparam.IndexBufferPtr = indexBuffer;
		dparam.ShaderPtr = shader;
		dparam.PixelShaderTextures[0] = texture;

		context->Draw(dparam);

		manager->EndScene();

		manager->EndRendering();

		manager->Present();

		// apply input
		joystick->RefreshInputState();

		if (joystick->IsPresent(0))
		{
			if (joystick->GetJoystickType(0) == ap::JoystickType::PS4)
			{
				if (joystick->GetButtonState(0, ap::JoystickButtonType::Circle) == ap::InputState::Push)
				{
					break;
				}
			}
		}
	}

	delete texture;

	delete context;
	
	delete shader;
	delete indexBuffer;
	delete vertexBuffer;


	delete manager;
	
	delete joystick;
	delete window;
	
	return 0;
}