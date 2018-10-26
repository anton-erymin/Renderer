#pragma once

#include <algorithm>

#include "Types.h"
#include "Scene.h"

#define IN
#define OUT
#define UNIFORM

class Shader {
public:
    virtual ~Shader() = default;

    const float_t *Call(const float_t *ins) {
        std::memcpy(inStart, ins, numInputBytes);
        Main();
        return static_cast<const float_t *>(outStart);
    }

    void SetIn(size_t numBytes, void *inputStart) {
        this->numInputBytes = numBytes;
        this->inStart = inputStart;
    }

    void SetOut(size_t numBytes, void *outStart) {
        this->numOutputBytes = numBytes;
        this->outStart = outStart;
    }

    size_t GetNumOutputBytes() const { return numOutputBytes; }

protected:
    virtual void Main() = 0;

    size_t numInputBytes;
    size_t numOutputBytes;
    void *inStart;
    const void *outStart;
};

class VertexShader : public Shader {
public:
    UNIFORM Mat4f model;
    UNIFORM Mat4f view;
    UNIFORM Mat4f projection;

    VertexShader() {
        SetIn(sizeof(Vec3f) * 2 + sizeof(Vec2f), &inPosition);
        SetOut(sizeof(Vec4f) + 2 * sizeof(Vec3f) + sizeof(Vec2f) + sizeof(float_t), &outClipPosition);
    }

    virtual void Main() override {
        outClipPosition = projection * view * model * Vec4f{inPosition, 1.0f};
        outWorldPosition = model * Vec4f{inPosition, 1.0f};
        outWorldNormal = model * Vec4f{inNormal, 0.0f};
        outUV = inUV;

        outDepthViewInverse = 1 / outClipPosition.w;
        outUV.x *= outDepthViewInverse;
        outUV.y *= outDepthViewInverse;
    }

private:
    IN Vec3f inPosition;
    IN Vec3f inNormal;
    IN Vec2f inUV;

    OUT Vec4f outClipPosition;
    OUT Vec3f outWorldPosition;
    OUT Vec3f outWorldNormal;
    OUT Vec2f outUV;

    OUT float_t outDepthViewInverse;
};

class FragmentShader : public Shader {
public:
    UNIFORM Material *material;

    FragmentShader() {
        SetIn(sizeof(Vec3f) * 2 + sizeof(Vec2f) + sizeof(float_t), &inWorldPosition);
        SetOut(sizeof(Vec4f), &outFragColor);
    }

    virtual void Main() override {
        inUV.x /= inDepthViewInverse;
        inUV.y /= inDepthViewInverse;

        Vec4f albedo = {1, 1, 1, 1};
        if (material) {
            if (material->albedoMap) {
                material->albedoMap->Sample(inUV.u, inUV.v, &albedo);
            } else {
                albedo = material->albedo;
            }
        }

        inWorldNormal.Normalize();
        Vec3f lightPos{0, 2, 1};
        Vec3f lightDir = Normalize(lightPos - inWorldPosition);
        outFragColor = albedo;// std::max(Dot(inWorldNormal, lightDir), 0.1f) * albedo;
    }

private:
    IN Vec3f inWorldPosition;
    IN Vec3f inWorldNormal;
    IN Vec2f inUV;
    IN float_t inDepthViewInverse;

    OUT Vec4f outFragColor;
};