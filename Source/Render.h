#pragma once

#include "Rasterizer.h"
#include "Mesh.h"
#include "Texture.h"

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

class Render {
public:
    bool IsFaceCulled(const Vec4f & v1, const Vec4f & v2, const Vec4f & v3);

    void RenderMesh(const Mesh &mesh, PolygonMode polygonMode, Texture &target, Texture *depthTarget);
	
private:
    Rasterizer rasterizer;
};