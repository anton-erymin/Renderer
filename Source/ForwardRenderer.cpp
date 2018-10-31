#include "ForwardRenderer.h"

#include "Math.h"

std::string SaveTexture(const Texture &texture) {
    auto fileName = texture.Name() + ".bmp";
    texture.SaveBmp(fileName);
    return fileName;
}

Texture& ForwardRenderer::Render(const Scene &scene, const Viewport &viewport) {
    framebuffer = std::make_unique<Texture>("FrameBuffer", width, height, 3, sizeof(float_t), false);
    depth = std::make_unique<Texture>("Depth", width, height, 1, sizeof(float_t));

    float_t clearValue[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //frameBuffer.Clear(clearValue);

    float_t depthClearValue[] = { 1.0f };
    depth->Clear(depthClearValue);

    Mat4f projection;
    Mat4f view;
    Mat4f model;

    Vec3f viewPosition{ 1.0f, 1.0f, 1.0f };
    view.LookAt(viewPosition, { 0.0f, 0.0f, 0.f }, { 0, 1, 0 });

    projection.PerspectiveProjection(45.0f, framebuffer->GetAspectRatio(), 0.01f, 0.1f);

    vertexShader.model = model;
    vertexShader.view = view;
    vertexShader.projection = projection;

    fragmentShader.viewPosition = viewPosition;
    fragmentShader.lights = &scene.lights;

    pipeline.RenderScene(scene, *framebuffer, depth.get(), vertexShader, fragmentShader, viewport);

    SaveTexture(*framebuffer);
    SaveTexture(*depth);

    return *framebuffer;
}
