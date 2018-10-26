#include "Mesh.h"

#include <stdexcept>
#include <filesystem>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Mesh::Mesh(const std::string &filename) {
    std::wstring wide{std::filesystem::path{filename}.parent_path()};

    using namespace tinyobj;
    attrib_t attrib;
    std::vector<shape_t> shapes;
    std::vector<material_t> materials;

    std::string err;
    bool ret = LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), (std::string{wide.begin(), wide.end()} +"\\").c_str());

    if (!err.empty() || !ret) {
        throw std::runtime_error{"Error while loading .obj: " + err};
    }

    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        vertices.push_back(Vec4f{Vec3f{&attrib.vertices[i]}, 1.0f});
    }
    for (size_t i = 0; i < attrib.normals.size(); i += 3) {
        normals.push_back(Vec4f{Vec3f{&attrib.normals[i]}});
    }
    for (size_t i = 0; i < attrib.texcoords.size(); i += 2) {
        uvs.push_back(Vec2f{attrib.texcoords[i], attrib.texcoords[i + 1]});
    }
    totalTriangles = 0;
    for (const auto &x : shapes) {
        submeshes.push_back({});
        auto &submesh = submeshes.back();
        submesh.name = x.name;
        for (size_t i = 0; i < x.mesh.indices.size(); i += 3) {
            Face face;
            face.vertexIndices[0] = x.mesh.indices[i    ].vertex_index;
            face.vertexIndices[1] = x.mesh.indices[i + 1].vertex_index;
            face.vertexIndices[2] = x.mesh.indices[i + 2].vertex_index;
            face.normalIndices[0] = x.mesh.indices[i].normal_index;
            face.normalIndices[1] = x.mesh.indices[i + 1].normal_index;
            face.normalIndices[2] = x.mesh.indices[i + 2].normal_index;
            face.uvIndices[0]     = x.mesh.indices[i].texcoord_index;
            face.uvIndices[1]     = x.mesh.indices[i + 1].texcoord_index;
            face.uvIndices[2]     = x.mesh.indices[i + 2].texcoord_index;
            submesh.faces.push_back(face);
        }
        totalTriangles += submesh.faces.size();
    }
}

std::ostream &operator<<(std::ostream &os, const Mesh &obj) {
	return os << "Mesh{" 
              << obj.name 
              << ", subs=" << obj.submeshes.size()
              << ", triangles=" << obj.totalTriangles << "}";
}
