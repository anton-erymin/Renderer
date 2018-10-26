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

    Scene scene;

    //scene.LoadMeshFromObj("Data\\Cube\\cube.obj");
    //scene.LoadMeshFromObj("Data/Sponza/sponza.obj");
    scene.LoadMeshFromObj("Data/Car/Car.obj");
    //scene.LoadMeshFromObj("Data\\CornellBox\\cornellbox.obj");

    Mat4f projection;
    Mat4f view;
    Mat4f model;

    view.LookAt({0.0f, 10.0f, 3.0f}, {0, 0, 0}, {0, 1, 0});

    projection.PerspectiveProjection(45.0f, frameBuffer.GetAspectRatio(), 0.01f, 10000.0f);

    Render render;
    VertexShader vertexShader;
    FragmentShader fragmentShader;

    vertexShader.model = model;
    vertexShader.view = view;
    vertexShader.projection = projection;

    Viewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = frameBuffer.Width();
    viewport.height = frameBuffer.Height();

    render.RenderScene(scene, frameBuffer, &depthTexture, vertexShader, fragmentShader, viewport);

    auto framebufferFileName = SaveTexture(frameBuffer);
    SaveTexture(depthTexture);

	system(framebufferFileName.c_str());

	std::cin.get();
	return 0;
}