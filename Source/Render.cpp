#include "Render.h"

#include "Types.h"
#include "Logger.h"



void Render::RenderMesh(const Mesh &mesh, PolygonMode polygonMode, Texture &target) {

    Mat4f projection;
    Mat4f view;
    Mat4f model;

    model.a34 = -1.0f;

    projection.PerspectiveProjection(45.0f, float_t(target.Width()) / target.Height(), 0.1f, 1000.0f);

    LOG(projection);

    Mat4f transform = projection * (view * model);

    for (const auto &submesh : mesh.GetSubmeshes()) {
        for (const auto &face : submesh.faces) {
            Vec4f v1 = transform * mesh.GetVertex(face.vertexIndices[0]);
            Vec4f v2 = transform * mesh.GetVertex(face.vertexIndices[1]);
            Vec4f v3 = transform * mesh.GetVertex(face.vertexIndices[2]);

            v1 *= 1 / v1.w;
            v2 *= 1 / v2.w;
            v3 *= 1 / v3.w;
            
            //LOG(v1 << v2 << v3);

            Vec2i vs1, vs2, vs3;
            vs1.x = (v1.x + 1) * 0.5f * target.Width();
            vs1.y = (v1.y + 1) * 0.5f * target.Height();
            vs2.x = (v2.x + 1) * 0.5f * target.Width();
            vs2.y = (v2.y + 1) * 0.5f * target.Height();
            vs3.x = (v3.x + 1) * 0.5f * target.Width();
            vs3.y = (v3.y + 1) * 0.5f * target.Height();

            DrawTriangleInfo info;
            info.v1.screenCoords = vs1;
            info.v2.screenCoords = vs2;
            info.v3.screenCoords = vs3;
            info.polygonMode = polygonMode;
            info.numAttributes = 0;
            rasterizer_.DrawTriangle(info, target);
        }
    }
}
