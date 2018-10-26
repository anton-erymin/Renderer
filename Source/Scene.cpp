#include "Scene.h"

#include <stdexcept>
#include <filesystem>
#include <map>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// The key for eliminating redundant vertices
class ObjKey
{
public:
    inline ObjKey() {}

    inline bool operator <(const ObjKey &other) const {
        if (positionIndex != other.positionIndex)
            return (positionIndex < other.positionIndex);
        if (normalIndex != other.normalIndex)
            return (normalIndex < other.normalIndex);
        if (texcoordsIndex != other.texcoordsIndex)
            return (texcoordsIndex < other.texcoordsIndex);
        return false;
    }

    uint32_t positionIndex;
    uint32_t normalIndex;
    uint32_t texcoordsIndex;
};

void Scene::LoadMeshFromObj(const std::string &filename) {
    std::wstring wide{std::filesystem::path{filename}.parent_path()};
    std::string root = std::string{wide.begin(), wide.end()} +"/";

    using namespace tinyobj;
    attrib_t attrib;
    std::vector<shape_t> shapes;
    std::vector<material_t> objMaterials;

    std::string err;
    bool ret = LoadObj(&attrib, &shapes, &objMaterials, &err, filename.c_str(), root.c_str());

    if (!err.empty() || !ret) {
        //throw std::runtime_error{"Error while loading .obj: " + err};
    }

    // Load the material data
    auto const base_material = static_cast<uint32_t>(objMaterials.size());
    materials.reserve(objMaterials.size());
    for (auto& material : objMaterials) {
        // Create the material object
        materials.push_back({});
        auto& m = materials.back();
        m.name = material.name;
        m.albedo = {material.diffuse[0], material.diffuse[1], material.diffuse[2]};

        if (!material.diffuse_texname.empty()) {
            auto fullPath = root + material.diffuse_texname;
            auto it = texturesMap.find(fullPath);
            if (it != texturesMap.end()) {
                m.albedoMap = &it->second;
            } else {
                Texture albedoTexture(fullPath);
                texturesMap[fullPath] = std::move(albedoTexture);
                m.albedoMap = &texturesMap[fullPath];
            }
        }
    }

    for (const auto &shape : shapes) {
        ObjKey objKey;
        std::vector<float> vertices;
        std::vector<uint32_t> indices;
        std::map<ObjKey, uint32_t> objMap;

        Material *material = nullptr;
        uint32_t i = 0, materialId = 0, vertexIndex = 0;
        float_t vertex[8];

        for (auto face = shape.mesh.num_face_vertices.begin(); face != shape.mesh.num_face_vertices.end(); i += *face, ++materialId, ++face) {
            // We only support triangle primitives
            if (*face != 3) {
                continue;
            }

            // Load the material information
            uint32_t material_idx = (!shape.mesh.material_ids.empty() ? shape.mesh.material_ids[materialId] : 0xffffffffu);
            material = (material_idx != 0xffffffffu ? &materials[material_idx] : nullptr);
            material_idx += (material_idx != 0xffffffffu ? base_material : 0);

            for (size_t v = 0u; v < 3u; ++v) {
                // Construct the lookup key
                objKey.positionIndex = shape.mesh.indices[i + v].vertex_index;
                objKey.normalIndex = shape.mesh.indices[i + v].normal_index;
                objKey.texcoordsIndex = shape.mesh.indices[i + v].texcoord_index;

                // Look up the vertex map
                uint32_t index;
                auto it = objMap.find(objKey);
                if (it != objMap.end()) {
                    index = it->second;
                } else {
                    // Push the vertex into memory 

                    for (size_t p = 0u; p < 3u; ++p) {
                        vertex[p] = attrib.vertices[3 * objKey.positionIndex + p];
                    }

                    if (objKey.normalIndex != 0xffffffffu) {
                        for (auto n = 0u; n < 3u; ++n) {
                            vertex[n + 3] = attrib.normals[3 * objKey.normalIndex+ n];
                        }
                    }
                        
                    if (objKey.texcoordsIndex != 0xffffffffu) {
                        for (auto t = 0u; t < 2u; ++t) {
                            vertex[t + 6] = attrib.texcoords[2 * objKey.texcoordsIndex + t];
                        }
                    }

                    /*for (auto c = 0u; c < 3u; ++c)
                        vertex[c + 9] = (material != nullptr ? material->diffuse[c] : 0.7f);*/

                    for (float value : vertex) {
                        vertices.push_back(value);
                    }

                    index = vertexIndex++;
                    objMap[objKey] = index;
                }

                // Push the index into memory
                indices.push_back(index);
            }
        }

        // Create the mesh object
        meshes.push_back(Mesh());
        auto& mesh = meshes.back();
        mesh.name = shape.name;
        std::swap(mesh.vertices, vertices);
        mesh.vertex_stride = sizeof(vertex);
        std::swap(mesh.indices, indices);
        mesh.index_stride = sizeof(uint32_t);
        mesh.material = material;
    }
}
