#pragma once

#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Types.h"
#include "Texture.h"

struct Material;

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

struct Material {
    std::string name;
    Vec3f albedo;
    const Texture *albedoMap{nullptr};
    Vec3f ambient;
};

struct Light {

};

struct Scene {
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::vector<Light> lights;
    
    std::unordered_map<std::string, Texture> texturesMap;

    void LoadMeshFromObj(const std::string &filename);
};
