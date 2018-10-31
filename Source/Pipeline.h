#pragma once

#include "Rasterizer.h"
#include "Scene.h"
#include "Texture.h"
#include "Shader.h"

enum class FrontFace {
    CW = 0,
    CCW = 1
};

enum class CullFace {
    None = 0,
    Front = 1,
    Back = 2,
    Both = 3
};

struct Viewport {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
};

class Pipeline {
public:
    void RenderScene(const Scene &scene, Texture &target, Texture *depthTarget, BaseVertexShader &vertexShader, PhongFragmentShader &fragmentShader, const Viewport &viewport);
    void RenderMesh(const Mesh &mesh, PolygonMode polygonMode, Texture &target, Texture *depthTarget, BaseVertexShader &vertexShader, PhongFragmentShader &fragmentShader, const Viewport &viewport);
	
private:
    Rasterizer rasterizer;
};