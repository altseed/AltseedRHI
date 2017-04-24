
#include "common.h"

void test_renderTarget(ar::GraphicsDeviceType device)
{
	auto window = ap::Window::Create();

	ap::WindowInitializationParameter wparam;
	wparam.WindowWidth = 640;
	wparam.WindowHeight = 480;
	wparam.ColorSpace = ap::ColorSpaceType::LinearSpace;
	wparam.GraphicsDevice = (ap::GraphicsDeviceType)device;

	window->Initialize(wparam);
	window->MakeContextCurrent();

	auto joystick = ap::Joystick::Create(window);

	auto manager = ar::Manager::Create(device);
	ar::ManagerInitializationParameter mparam;

	mparam.Handles[0] = window->GetHandle();
	mparam.WindowWidth = 640;
	mparam.WindowHeight = 480;
	mparam.ColorSpace = ar::ColorSpaceType::LinearSpace;
	manager->Initialize(mparam);

	auto vertexBuffer = ar::VertexBuffer::Create(manager);
	vertexBuffer->Initialize(manager, sizeof(SimpleVertex), 4, false);

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

	for (int32_t i = 0; i < 4; i++)
	{
		vbuffer[i].r = 255;
		vbuffer[i].g = 255;
		vbuffer[i].b = 255;
		vbuffer[i].a = 255;
	}

	vertexBuffer->Write(vbuffer.data(), sizeof(SimpleVertex) * 4);

	auto indexBuffer = ar::IndexBuffer::Create(manager);
	indexBuffer->Initialize(manager, 6, false);

	std::array<int16_t, 6> ibuffer;
	ibuffer[0] = 0;
	ibuffer[1] = 1;
	ibuffer[2] = 2;
	ibuffer[3] = 0;
	ibuffer[4] = 2;
	ibuffer[5] = 3;

	indexBuffer->Write(ibuffer.data(), sizeof(int16_t) * 6);

	auto shader = ar::Shader::Create(manager);

	std::vector <ar::VertexLayout> vertexLayouts;
	vertexLayouts.resize(3);
	vertexLayouts[0].Name = "Pos";
	vertexLayouts[0].LayoutFormat = ar::VertexLayoutFormat::R32G32B32_FLOAT;
	vertexLayouts[1].Name = "UV";
	vertexLayouts[1].LayoutFormat = ar::VertexLayoutFormat::R32G32_FLOAT;
	vertexLayouts[2].Name = "Color";
	vertexLayouts[2].LayoutFormat = ar::VertexLayoutFormat::R8G8B8A8_UNORM;

	std::vector<uint8_t> shader_vs;
	std::vector<uint8_t> shader_ps;

	LoadShaderFile(shader_vs, "texture2d_VS.dat", device);
	LoadShaderFile(shader_ps, "texture2d_PS.dat", device);

	ar::ShaderCompilerParameter compilerParam;
	ar::ShaderCompilerResult compilerResult;

	if (manager->GetDeviceType() == ar::GraphicsDeviceType::OpenGL)
	{
		auto compiler = ar::Compiler::Create(manager);
		compilerParam.OpenGLVersion = ar::OpenGLVersionType::OpenGL33;
		compilerParam.VertexShaderTexts.push_back((char*)shader_vs.data());
		compilerParam.PixelShaderTexts.push_back((char*)shader_ps.data());
		compiler->Compile(compilerResult, compilerParam);
		delete compiler;
	}
	else
	{
		compilerResult.VertexShaderBuffer = shader_vs;
		compilerResult.PixelShaderBuffer = shader_ps;
	}

	shader->Initialize(manager, compilerResult, vertexLayouts, false);

	auto context = ar::Context::Create(manager);
	context->Initialize(manager);

	std::vector<uint8_t> textureBuf;
	LoadFile(textureBuf, "Data/Texture01.png");
	ar::Texture2D* texture = ar::Texture2D::Create(manager);
	texture->Initialize(manager, textureBuf.data(), textureBuf.size(), false, true);

	ar::RenderTexture2D* renderTarget = ar::RenderTexture2D::Create(manager);
	renderTarget->Initialize(manager, 128, 128, ar::TextureFormat::R8G8B8A8_UNORM);

	while (window->DoEvent())
	{
		manager->BeginRendering();

		ar::SceneParameter sparam_offscreen;
		sparam_offscreen.RenderTargets[0] = renderTarget;
		manager->BeginScene(sparam_offscreen);

		ar::DrawParameter dparam_offscreen;
		dparam_offscreen.VertexBufferPtr = vertexBuffer;
		dparam_offscreen.IndexBufferPtr = indexBuffer;
		dparam_offscreen.ShaderPtr = shader;
		dparam_offscreen.PixelShaderTextures[0] = texture;

		context->Begin();
		context->Draw(dparam_offscreen);
		context->End();

		manager->EndScene();


		ar::SceneParameter sparam;
		manager->BeginScene(sparam);

		ar::DrawParameter dparam;
		dparam.VertexBufferPtr = vertexBuffer;
		dparam.IndexBufferPtr = indexBuffer;
		dparam.ShaderPtr = shader;
		dparam.PixelShaderTextures[0] = renderTarget;

		context->Begin();
		context->Draw(dparam);
		context->End();

		manager->EndScene();

		manager->EndRendering();

		manager->Present();

		window->Present();

		// apply input
		joystick->RefreshInputState();

		if (joystick->IsPresent(0))
		{
			if (joystick->GetJoystickType(0) == ap::JoystickType::PS4)
			{
				if (joystick->GetButtonState(0, ap::JoystickButtonType::RightRight) == ap::InputState::Push)
				{
					break;
				}
			}
		}
	}

	delete renderTarget;

	delete texture;

	delete context;

	delete shader;
	delete indexBuffer;
	delete vertexBuffer;

	delete manager;

	delete joystick;

	window->MakeContextNone();
	delete window;
}