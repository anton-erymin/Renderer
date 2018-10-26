#include "Render.h"

#include "Types.h"
#include "Logger.h"

bool Render::IsFaceCulled(const Vec4f &v1, const Vec4f &v2, const Vec4f &v3) {
    return ((v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (v2.y - v1.y)) < 0;
}

void Render::RenderMesh(const Mesh &mesh, PolygonMode polygonMode, Texture &target, Texture *depthTarget) {

    Mat4f projection;
    Mat4f view;
    Mat4f model;

    view.LookAt({3.f, 1, 4}, {0, 1, 0}, {0, 1, 0});

    projection.PerspectiveProjection(45.0f, float_t(target.Width()) / target.Height(), 0.1f, 1.3f);


    LOG(projection);

    Mat4f transform = projection * view * model;

    for (const auto &submesh : mesh.GetSubmeshes()) {
        for (const auto &face : submesh.faces) {
            Vec4f v1 = transform * mesh.GetVertex(face.vertexIndices[0]);
            Vec4f v2 = transform * mesh.GetVertex(face.vertexIndices[1]);
            Vec4f v3 = transform * mesh.GetVertex(face.vertexIndices[2]);

            Vec3f n1 = model * mesh.GetNormal(face.normalIndices[0]);
            Vec3f n2 = model * mesh.GetNormal(face.normalIndices[1]);
            Vec3f n3 = model * mesh.GetNormal(face.normalIndices[2]);

            // Clipping
            if (v1.x < -v1.w || v1.x > v1.w) continue;
            if (v2.x < -v2.w || v2.x > v2.w) continue;
            if (v3.x < -v3.w || v3.x > v3.w) continue;
            if (v1.y < -v1.w || v1.y > v1.w) continue;
            if (v2.y < -v2.w || v2.y > v2.w) continue;
            if (v3.y < -v3.w || v3.y > v3.w) continue;
            if (v1.z < -v1.w || v1.z > v1.w) continue;
            if (v2.z < -v2.w || v2.z > v2.w) continue;
            if (v3.z < -v3.w || v3.z > v3.w) continue;

            // Perspective division by w
            v1 *= 1 / v1.w;
            v2 *= 1 / v2.w;
            v3 *= 1 / v3.w;
            
            if (IsFaceCulled(v1, v2, v3)) {
                continue;
            }

            //LOG(v1 << v2 << v3);

            // Viewport transform
            Vec2i vs1, vs2, vs3;
            vs1.x = (v1.x + 1) * 0.5f * target.Width();
            vs1.y = (-v1.y + 1) * 0.5f * target.Height();
            vs2.x = (v2.x + 1) * 0.5f * target.Width();
            vs2.y = (-v2.y + 1) * 0.5f * target.Height();
            vs3.x = (v3.x + 1) * 0.5f * target.Width();
            vs3.y = (-v3.y + 1) * 0.5f * target.Height();

            float_t v1Attributes[RASTERIZER_MAX_ATTRIBUTES];
            float_t v2Attributes[RASTERIZER_MAX_ATTRIBUTES];
            float_t v3Attributes[RASTERIZER_MAX_ATTRIBUTES];

            size_t numAttributes = 0;
            if (depthTarget) {
                v1Attributes[numAttributes] = v1.z;
                v2Attributes[numAttributes] = v2.z;
                v3Attributes[numAttributes] = v3.z;
                ++numAttributes;
            }

            v1Attributes[numAttributes + 0] = n1.x;
            v1Attributes[numAttributes + 1] = n1.y;
            v1Attributes[numAttributes + 2] = n1.z;
            v2Attributes[numAttributes + 0] = n2.x;
            v2Attributes[numAttributes + 1] = n2.y;
            v2Attributes[numAttributes + 2] = n2.z;
            v3Attributes[numAttributes + 0] = n3.x;
            v3Attributes[numAttributes + 1] = n3.y;
            v3Attributes[numAttributes + 2] = n3.z;
            numAttributes += 3;

            DrawTriangleInfo drawTriangleInfo;
            drawTriangleInfo.v1.screenCoords = vs1;
            drawTriangleInfo.v2.screenCoords = vs2;
            drawTriangleInfo.v3.screenCoords = vs3;
            drawTriangleInfo.v1.attributes = v1Attributes;
            drawTriangleInfo.v2.attributes = v2Attributes;
            drawTriangleInfo.v3.attributes = v3Attributes;
            drawTriangleInfo.polygonMode = polygonMode;
            drawTriangleInfo.numAttributes = numAttributes;
            rasterizer.DrawTriangle(drawTriangleInfo, target, depthTarget);
        }
    }
}
