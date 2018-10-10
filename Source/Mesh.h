#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "Types.h"

struct Face {
    size_t vertexIndices[3];
    size_t normalIndices[3];
    size_t uvIndices[3];
};

struct SubMesh {
    std::string name;
    std::vector<Face> faces;
    size_t materialIndex;
};

class Mesh {
public:
    explicit Mesh(const std::string &filename);

    const Vec4f &GetVertex(size_t i) const { return vertices_[i]; }
    const Vec4f &GetNormal(size_t i) const { return normals_[i]; }
    const Vec2f &GetUV(size_t i) const { return uvs_[i]; }

    const std::vector<SubMesh> &GetSubmeshes() const { return submeshes_; }

private:
    friend std::ostream &operator<<(std::ostream &os, const Mesh &obj);

    std::string name_;

    std::vector<Vec4f> vertices_;
    std::vector<Vec4f> normals_;
    std::vector<Vec2f> uvs_;

    std::vector<SubMesh> submeshes_;

    size_t totalTriangles_;
};