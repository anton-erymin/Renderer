#include "Pipeline.h"

#include "Types.h"
#include "Logger.h"
#include "Shader.h"
#include "Algorithms.h"

void Pipeline::RenderScene(const Scene &scene, Texture &target, Texture *depthTarget, BaseVertexShader &vertexShader, PhongFragmentShader &fragmentShader, const Viewport &viewport) {
    for (const auto &mesh : scene.meshes) {
        fragmentShader.material = mesh.material;
        RenderMesh(mesh, PolygonMode::Fill, target, depthTarget, vertexShader, fragmentShader, viewport);
    }

    //GaussianBlur(target, { 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f }, 5);
    //Blur(target, 100);

    if (!target.IsNormalized()) {
        ReinhardTonemapping(target);
    }

    float_t gamma = 2.2f;
    float_t gammaInv = 1.0f / gamma;
    // Compress gamma
    GammaCorrection(target, gammaInv);
}

void Pipeline::RenderMesh(const Mesh &mesh, PolygonMode polygonMode, Texture &target, Texture *depthTarget, BaseVertexShader &vertexShader, PhongFragmentShader &fragmentShader, const Viewport &viewport) {
    size_t i = 0;
    for (size_t primIndex = 0; primIndex < mesh.GetPrimCount(); primIndex++) {
        uint32_t indices[] = {mesh.indices[i], mesh.indices[i + 1], mesh.indices[i + 2]};
        i += 3;
 
        Vec4f NDCs[3];
        float_t shaderResults[3][64];
        float_t vertexAttributes[3][RASTERIZER_MAX_ATTRIBUTES];
        size_t numAttributes = 0;
        Vec2i screen[3];

        size_t attributesBytes = vertexShader.GetNumOutputBytes() - sizeof(Vec4f);

        bool vertexClipped = false;
        size_t v = 0;
        for (const auto index : indices) {
            // Get vertex data pointer
            const float_t *vertexData = &mesh.vertices[index * mesh.vertex_stride / sizeof(float_t)];
            // Calling vertex shader
            const float_t *shaderResult = vertexShader.Call(vertexData);
            std::memcpy(shaderResults[v], shaderResult, vertexShader.GetNumOutputBytes());
            // Get clipped result
            Vec4f clipped{shaderResult};

            // Clipping
            if (clipped.x < -clipped.w || clipped.x > clipped.w) {
                vertexClipped |= true;
            }
            if (clipped.y < -clipped.w || clipped.y > clipped.w) {
                vertexClipped |= true;
            }
            if (clipped.z < -clipped.w || clipped.z > clipped.w) {
                vertexClipped |= true;
                break;
            }

            // Perspective division by w
            NDCs[v] = clipped * (1 / clipped.w);
            ++v;
        }
        if (vertexClipped) {
            continue;
        }
    
        // Face culling
        if (!IsFaceCCW(NDCs[0], NDCs[1], NDCs[2])) {
            continue;
        }

        for (size_t v = 0; v < 3; v++) {
            // Viewport transform
            screen[v].x = int32_t(std::roundf((NDCs[v].x + 1) * 0.5f * viewport.width + viewport.x));
            screen[v].y = int32_t(std::roundf((-NDCs[v].y + 1) * 0.5f * viewport.height + viewport.y));

            // Fill in attributes
            numAttributes = 0;

            // Depth
            if (depthTarget) {
                vertexAttributes[v][numAttributes] = NDCs[v].z;
                ++numAttributes;
            }

            // Copy shader out except for main clipped out
            std::memcpy(&vertexAttributes[v][numAttributes], shaderResults[v] + 4, attributesBytes);
            numAttributes += attributesBytes / sizeof(float_t);
        }

        DrawTriangleInfo drawTriangleInfo;
        drawTriangleInfo.v1.screenCoords = screen[0];
        drawTriangleInfo.v2.screenCoords = screen[1];
        drawTriangleInfo.v3.screenCoords = screen[2];

        drawTriangleInfo.v1.attributes = vertexAttributes[0];
        drawTriangleInfo.v2.attributes = vertexAttributes[1];
        drawTriangleInfo.v3.attributes = vertexAttributes[2];

        drawTriangleInfo.polygonMode = polygonMode;
        drawTriangleInfo.numAttributes = numAttributes;
        rasterizer.DrawTriangle(drawTriangleInfo, target, depthTarget, fragmentShader);
    }

}
