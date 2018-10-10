#pragma once

#include "Rasterizer.h"
#include "Mesh.h"
#include "Texture.h"

class Render {
public:
    void RenderMesh(const Mesh &mesh, PolygonMode polygonMode, Texture &target);
	
private:
    Rasterizer rasterizer_;
};