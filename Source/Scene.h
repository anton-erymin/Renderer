#pragma once

#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Types.h"
#include "Texture.h"

// Describing scene

struct Material;

/* Mesh class */
struct Mesh {
    std::string name;

    std::vector<float_t> vertices;
    uint32_t vertex_stride;
    std::vector<uint32_t> indices;
    uint32_t index_stride;

    Material *material{nullptr};

    // Gets the number of primitives in the mesh
    inline uint32_t GetPrimCount() const {
        return GetIndexCount() / 3;
    }

    // Gets the number of indices in the mesh
    inline uint32_t GetIndexCount() const {
        return static_cast<uint32_t>((indices.size() << 2) / index_stride);
    }

    // Gets the number of vertices in the mesh
    inline uint32_t GetVertexCount() const {
        return static_cast<uint32_t>((vertices.size() << 2) / vertex_stride);
    }
};

/* Material */
struct Material {
    std::string name;
    Vec3f ambient{0.1f, 0.1f, 0.1f};
    Vec3f albedo{1.0f, 1.0f, 1.0f};
    Vec3f specular{1.0f, 1.0f, 1.0f};
    Vec3f emission{0, 0, 0};
    float_t shininess;
    const Texture *ambientMap{nullptr};
    const Texture *albedoMap{nullptr};
    const Texture *specularMap{nullptr};
    const Texture *emissionMap{ nullptr };
};

enum class LightType {
    Directional = 0,
    Omni = 1
};

/* Light */
struct Light {
    LightType lightType;
    union {
        Vec3f direction;
        Vec3f position;
    };
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;

    float_t constant{1.0f};
    float_t linear{1.0f};
    float_t quadratic{1.0f};
};

struct Scene {
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<Light> lights;
    
    std::unordered_map<std::string, Texture> texturesMap;

    void LoadMeshFromObj(const std::string &filename, bool scaleToUnit = true, bool invertTexU = false, bool invertTexV = false);
};
