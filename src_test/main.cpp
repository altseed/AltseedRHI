
#include <stdio.h>

#include <ap.Window.h>
#include <ar.Base.h>

#include <iostream>
#include <fstream>

#if defined(_OTHER)

#else

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

int main()
{
	auto window = ap::Window::Create();
	
	ap::WindowInitializationParameter wparam;
	wparam.WindowWidth = 640;
	wparam.WindowHeight = 480;
	wparam.GraphicsDevice = ap::GraphicsDeviceType::DirectX11;

	window->Initialize(wparam);

	auto manager = ar::Manager::Create();

	ar::ManagerInitializationParameter mparam;
	mparam.Handles[0] = window->GetHandle();
	mparam.WindowWidth = 640;
	mparam.WindowHeight = 480;
	manager->Initialize(mparam);

	auto context = ar::Context::Create();
	context->Initialize(manager);

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
	vbuffer[3].r = 255;
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

	std::ifstream fin_vs("HLSL/simple_VS.dat", std::ios::in | std::ios::binary);
	std::vector<uint8_t> shader_vs;
	while (!fin_vs.eof())
	{
		uint8_t d;
		fin_vs.read((char *)&d, 1);
		shader_vs.push_back(d);
	}
	shader_vs.pop_back();

	fin_vs.close();

	std::ifstream fin_ps("HLSL/simple_PS.dat", std::ios::in | std::ios::binary);
	std::vector<uint8_t> shader_ps;
	while (!fin_ps.eof())
	{
		uint8_t d;
		fin_ps.read((char *)&d, 1);
		shader_ps.push_back(d);
	}
	shader_ps.pop_back();

	fin_ps.close();

	shader->Initialize(manager, shader_vs.data(), shader_vs.size(), shader_ps.data(), shader_ps.size(), vertexLayouts);

	while (window->DoEvent())
	{
		manager->BeginRendering();

		ar::SceneParameter sparam;

		manager->BeginScene(sparam);

		ar::DrawParameter dparam;
		dparam.VertexBufferPtr = vertexBuffer;
		dparam.IndexBufferPtr = indexBuffer;
		dparam.ShaderPtr = shader;

		context->Draw(dparam);

		manager->EndScene();

		manager->EndRendering();

		manager->Present();
	}

	delete shader;
	delete indexBuffer;
	delete vertexBuffer;

	delete context;
	delete manager;
	delete window;

	return 0;
}