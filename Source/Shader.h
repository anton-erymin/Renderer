#pragma once

#include <algorithm>
#include <cassert>

#include "Types.h"
#include "Scene.h"

#define IN
#define OUT
#define UNIFORM

/* Base shader class. */
class Shader {
public:
    virtual ~Shader() = default;

    const float_t *Call(const float_t *ins) {
        assert(ins);
        std::memcpy(inStart, ins, numInputBytes);
        Main();
        return static_cast<const float_t *>(outStart);
    }

    /* How many bytes is as in? */
    void SetIn(size_t numBytes, void *inputStart) {
        assert(numBytes > 0);
        assert(inputStart);
        this->numInputBytes = numBytes;
        this->inStart = inputStart;
    }

    /* How many bytes is as out? */
    void SetOut(size_t numBytes, void *outStart) {
        assert(numBytes > 0);
        assert(outStart);
        this->numOutputBytes = numBytes;
        this->outStart = outStart;
    }

    size_t GetNumOutputBytes() const { return numOutputBytes; }

protected:
    /* Shader user entry point. */
    virtual void Main() = 0;

    size_t numInputBytes;
    size_t numOutputBytes;
    void *inStart;
    const void *outStart;
};

/* Basic vertex shader. */
class BaseVertexShader : public Shader {
public:
    UNIFORM Mat4f model;
    UNIFORM Mat4f view;
    UNIFORM Mat4f projection;

    BaseVertexShader() {
        SetIn(sizeof(Vec3f) * 2 + sizeof(Vec2f), &inPosition);
        SetOut(sizeof(Vec4f) + 2 * sizeof(Vec3f) + sizeof(Vec2f) + sizeof(float_t), &outClipPosition);
    }

    virtual void Main() override;

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

/* Basic phong fragment shader. */
class PhongFragmentShader : public Shader {
public:
    UNIFORM Material *material;
    UNIFORM const std::vector<Light> *lights;
    UNIFORM Vec3f viewPosition;

    PhongFragmentShader() {
        SetIn(sizeof(Vec3f) * 2 + sizeof(Vec2f) + sizeof(float_t), &inWorldPosition);
        SetOut(sizeof(Vec4f), &outFragColor);
    }

    virtual void Main() override;

private:
    IN Vec3f inWorldPosition;
    IN Vec3f inWorldNormal;
    IN Vec2f inUV;
    IN float_t inDepthViewInverse;

    OUT Vec4f outFragColor;
};