#include <iostream>

#include "Logger.h"
#include "Render.h"

#define WIDTH    (1600)
#define HEIGHT   (1200)

std::string SaveTexture(const Texture &texture) {
    auto fileName = texture.Name() + ".bmp";
    texture.SaveBmp(fileName);
    return fileName;
}

int main() {
	Texture frameBuffer{ "FrameBuffer", WIDTH, HEIGHT, 3, sizeof(float_t) };
    Texture depthTexture{"Depth", WIDTH, HEIGHT, 1, sizeof(float_t)};

    float_t depthClearValue[] = {1.0f};
    depthTexture.Clear(depthClearValue);

    //Mesh mesh{"Data\\Cube\\cube.obj"};
    //Mesh mesh{"Data\\CornellBox\\cornellbox.obj"};
    Mesh mesh{"Data/Car/Car.obj"};
    LOG(mesh);

    Render render;

    render.RenderMesh(mesh, PolygonMode::Fill, frameBuffer, &depthTexture);

    auto framebufferFileName = SaveTexture(frameBuffer);
    SaveTexture(depthTexture);

	system(framebufferFileName.c_str());

	std::cin.get();
	return 0;
}