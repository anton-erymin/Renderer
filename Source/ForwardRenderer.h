#pragma once

#include <memory>

#include "Texture.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Scene.h"

class ForwardRenderer {
public:
    ForwardRenderer(size_t width, size_t height) : width(width), height(height) {}

    Texture& Render(const Scene &scene, const Viewport &viewport);

private:
    size_t width;
    size_t height;

    // Render targets
    std::unique_ptr<Texture> framebuffer;
    std::unique_ptr<Texture> depth;

    // Pipeline
    Pipeline pipeline;

    // Shaders
    BaseVertexShader vertexShader;
    PhongFragmentShader fragmentShader;
};