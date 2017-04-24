
#include "common.h"

void test_simple(ar::GraphicsDeviceType device);
void test_texture2d(ar::GraphicsDeviceType device);
void test_renderTarget(ar::GraphicsDeviceType device);


int main()
{
	test_renderTarget(ar::GraphicsDeviceType::DirectX11);
	return 0;
}