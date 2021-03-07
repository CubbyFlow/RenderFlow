#include <GL3/AssetLoader.hpp>
#include <GL3/DebugUtils.hpp>
#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

inline bool HasSmoothingGroup(const tinyobj::shape_t& shape)
{
    for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++)
    {
        if (shape.mesh.smoothing_group_ids[i] > 0)
        {
            return true;
        }
    }
    return false;
}

bool CheckTriangle(const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3)
{
    return (v2.x - v1.x) * (v3.y - v2.y) != (v3.x - v2.x) * (v2.y - v1.y);
}

glm::vec3 CalculateNormal(const glm::vec3 v1, const glm::vec3 v2, const glm::vec3 v3)
{
    if (!CheckTriangle(v1, v2, v3))
        return glm::vec3(0.0f);

    glm::vec3 edge1 = v2 - v1;
    glm::vec3 edge2 = v3 - v2;
    return glm::normalize(glm::cross(edge1, edge2));
}

void ComputeSmoothingNormals(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape, std::map<int, glm::vec3>& smoothVertexNormals)
{
    smoothVertexNormals.clear();
    const auto& vertices = attrib.vertices;

    for (size_t faceIndex = 0; faceIndex < shape.mesh.indices.size() / 3; faceIndex++)
    {
        // Get the three indexes of the face (all faces are triangular)
        tinyobj::index_t idx0 = shape.mesh.indices[3 * faceIndex + 0];
        tinyobj::index_t idx1 = shape.mesh.indices[3 * faceIndex + 1];
        tinyobj::index_t idx2 = shape.mesh.indices[3 * faceIndex + 2];

        // Get the three vertex indexes and coordinates
        glm::vec3 position[3];  // coordinates

        int f0 = idx0.vertex_index;
        int f1 = idx1.vertex_index;
        int f2 = idx2.vertex_index;
        assert(f0 >= 0 && f1 >= 0 && f2 >= 0);

        position[0] = glm::vec3(vertices[3 * f0], vertices[3 * f0 + 1], vertices[3 * f0 + 2]);
        position[1] = glm::vec3(vertices[3 * f1], vertices[3 * f1 + 1], vertices[3 * f1 + 2]);
        position[2] = glm::vec3(vertices[3 * f2], vertices[3 * f2 + 1], vertices[3 * f2 + 2]);

        // Compute the normal of the face
        glm::vec3 normal = CalculateNormal(position[0], position[1], position[2]);

        // Add the normal to the three vertexes
        int faces[3] = { f0, f1, f2 };
        for (size_t i = 0; i < 3; ++i)
        {
            auto iter = smoothVertexNormals.find(faces[i]);
            if (iter == smoothVertexNormals.end())
            {
                smoothVertexNormals[faces[i]] = normal;
            }
            else
            {
                iter->second += normal;
            }
        }
    }  // f

    // Normalize the normals, that is, make them unit vectors
    for (auto& p : smoothVertexNormals)
    {
        p.second = glm::normalize(p.second);
    }
}

constexpr float EPSILON = 1e-6f;

struct PackedVertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;

    PackedVertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 n)
        : position(pos), texCoord(uv), normal(n) {};
};

//! lexicographically sorting vector.
inline bool operator<(const PackedVertex& v1, const PackedVertex& v2)
{
    if (std::fabs(v1.position.x - v2.position.x) >= 0.001f) return v1.position.x < v2.position.x;
    if (std::fabs(v1.position.y - v2.position.y) >= 0.001f) return v1.position.y < v2.position.y;
    if (std::fabs(v1.position.z - v2.position.z) >= 0.001f) return v1.position.z < v2.position.z;
    if (std::fabs(v1.texCoord.x - v2.texCoord.x) >= 0.1f) return v1.texCoord.x < v2.texCoord.x;
    if (std::fabs(v1.texCoord.y - v2.texCoord.y) >= 0.1f) return v1.texCoord.y < v2.texCoord.y;
    if (std::fabs(v1.normal.x - v2.normal.x) >= 0.3f) return v1.normal.x < v2.normal.x;
    if (std::fabs(v1.normal.y - v2.normal.y) >= 0.3f) return v1.normal.y < v2.normal.y;
    if (std::fabs(v1.normal.z - v2.normal.z) >= 0.3f) return v1.normal.z < v2.normal.z;
    return false;
}

namespace GL3 {

    bool AssetLoader::LoadObjFile(const std::string& path, std::vector<float>& vertices, std::vector<unsigned int>& indices, VertexFormat format)
	{
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        //! Load obj file with tinyobjloader 
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
        if (!ret)
        {
            std::cerr << "Failed to load " << path << std::endl;
            StackTrace::PrintStack();
            return false;
        }
        if (shapes.size() == 0)
        {
            std::cerr << "No shapes in " << path << std::endl;
            StackTrace::PrintStack();
            return false;
        }

        for (auto& shape : shapes)
        {
            std::map<int, glm::vec3> smoothVertexNormals;
            if (HasSmoothingGroup(shape))
            {
                ComputeSmoothingNormals(attrib, shape, smoothVertexNormals);
            }
            
            std::map<PackedVertex, unsigned int> packedVerticesMap;
            for (size_t faceIndex = 0; faceIndex < shape.mesh.indices.size() / 3; ++faceIndex)
            {
                /*
                idx0 (pos (float3), normal(float3), texcoords(float2))
                |\
                | \
                |  \
                |   \ idx2 (pos (float3), normal(float3), texcoords(float2))
                |   /
                |  /
                | /
                |/
                idx1 (pos (float3), normal(float3), texcoords(float2))
                */
                tinyobj::index_t idx0 = shape.mesh.indices[3 * faceIndex + 0];
                tinyobj::index_t idx1 = shape.mesh.indices[3 * faceIndex + 1];
                tinyobj::index_t idx2 = shape.mesh.indices[3 * faceIndex + 2];

                glm::vec3 position[3];
                glm::vec2 texCoord[3];
                glm::vec3 normal[3];

                if (static_cast<int>(format & VertexFormat::Position3))
                {
                    for (int k = 0; k < 3; k++)
                    {
                        int f0 = idx0.vertex_index;
                        int f1 = idx1.vertex_index;
                        int f2 = idx2.vertex_index;
                        assert(f0 >= 0 && f1 >= 0 && f2 >= 0);

                        position[0][k] = attrib.vertices[3 * f0 + k];
                        position[1][k] = attrib.vertices[3 * f1 + k];
                        position[2][k] = attrib.vertices[3 * f2 + k];
                    }
                }

                if (static_cast<int>(format & VertexFormat::Normal3))
                {
                    bool invalidNormal = false;
                    if (attrib.normals.size() > 0)
                    {
                        int f0 = idx0.normal_index;
                        int f1 = idx1.normal_index;
                        int f2 = idx2.normal_index;
                        if (f0 < 0 || f1 < 0 || f2 < 0)
                        {
                            invalidNormal = true;
                        }
                        else
                        {
                            for (size_t k = 0; k < 3; k++)
                            {
                                assert(size_t(3 * f0 + k) < attrib.normals.size());
                                assert(size_t(3 * f1 + k) < attrib.normals.size());
                                assert(size_t(3 * f2 + k) < attrib.normals.size());
                                normal[0][k] = attrib.normals[3 * f0 + k];
                                normal[1][k] = attrib.normals[3 * f1 + k];
                                normal[2][k] = attrib.normals[3 * f2 + k];
                            }
                        }
                    }
                    else
                    {
                        invalidNormal = true;
                    }
                    if (invalidNormal)
                    {
                        if (!smoothVertexNormals.empty())
                        {
                            //! Use smoothing normals
                            int f0 = idx0.vertex_index;
                            int f1 = idx1.vertex_index;
                            int f2 = idx2.vertex_index;
                            if (f0 >= 0 && f1 >= 0 && f2 >= 0)
                            {
                                normal[0] = smoothVertexNormals[f0];
                                normal[1] = smoothVertexNormals[f1];
                                normal[2] = smoothVertexNormals[f2];
                            }
                        }
                        else
                        {
                            normal[0] = CalculateNormal(position[0], position[1], position[2]);
                            normal[1] = normal[0];
                            normal[2] = normal[0];
                        }
                    }
                }
                
                if (static_cast<int>(format & VertexFormat::TexCoord2))
                {
                    if (attrib.texcoords.size() > 0)
                    {
                        int f0 = idx0.texcoord_index;
                        int f1 = idx1.texcoord_index;
                        int f2 = idx2.texcoord_index;

                        if (f0 < 0 || f1 < 0 || f2 < 0)
                        {
                            texCoord[0] = glm::vec2(0.0f, 0.0f);
                            texCoord[1] = glm::vec2(0.0f, 0.0f);
                            texCoord[2] = glm::vec2(0.0f, 0.0f);
                        }
                        else
                        {
                            assert(attrib.texcoords.size() > size_t(2 * f0 + 1));
                            assert(attrib.texcoords.size() > size_t(2 * f1 + 1));
                            assert(attrib.texcoords.size() > size_t(2 * f2 + 1));

                            //! Flip Y coord.
                            texCoord[0] = glm::vec2(attrib.texcoords[2 * f0], 1.0f - attrib.texcoords[2 * f0 + 1]);
                            texCoord[1] = glm::vec2(attrib.texcoords[2 * f1], 1.0f - attrib.texcoords[2 * f1 + 1]);
                            texCoord[2] = glm::vec2(attrib.texcoords[2 * f2], 1.0f - attrib.texcoords[2 * f2 + 1]);
                        }
                    }
                    else
                    {
                        texCoord[0] = glm::vec2(0.0f, 0.0f);
                        texCoord[1] = glm::vec2(0.0f, 0.0f);
                        texCoord[2] = glm::vec2(0.0f, 0.0f);
                    }
                }
                
                //! From now on, vertices in one face allocated.
                for (unsigned int k = 0; k < 3; ++k)
                {
                    PackedVertex vertex(position[k], texCoord[k], normal[k]);

                    auto iter = packedVerticesMap.find(vertex);

                    if (iter == packedVerticesMap.end())
                    {
                        if (static_cast<int>(format & VertexFormat::Position3))
                            vertices.insert(vertices.end(), { position[k].x, position[k].y, position[k].z });
                        if (static_cast<int>(format & VertexFormat::Normal3))
                            vertices.insert(vertices.end(), { normal[k].x, normal[k].y, normal[k].z });
                        if (static_cast<int>(format & VertexFormat::TexCoord2))
                            vertices.insert(vertices.end(), { texCoord[k].x, texCoord[k].y });
                        unsigned int newIndex = static_cast<unsigned int>(vertices.size() - 1);
                        indices.push_back(newIndex);
                        packedVerticesMap[vertex] = newIndex;
                    }
                    else
                    {
                        indices.push_back(iter->second);
                    }
                }
            }
        }

        return true;
    }

    bool AssetLoader::LoadRawFile(const std::string& path, std::vector<char>& data)
    {
        std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
        if (!file.is_open())
            return false;

        const size_t fileSize = file.tellg();
        data.resize(fileSize);
        file.seekg(std::ios::beg);
        file.read(&data[0], fileSize);
        
        file.close();

        return true;
    }

}; //! end of Mesh.cpp