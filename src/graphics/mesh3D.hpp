#pragma once

#include <glm/glm.hpp>

// std
#include <vector>
#include <memory>

class Mesh3D {

public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 tangent;

        Vertex(glm::vec3 position,
               glm::vec3 normal,
               glm::vec2 texCoords,
               glm::vec3 tangent) :
               position{position},
               normal{normal},
               texCoords{texCoords},
               tangent{tangent} {}
    };

    Mesh3D() {}

    Mesh3D(const std::vector<Mesh3D::Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           bool buffer);

    ~Mesh3D();

    void BindVAO();
    void UnbindVAO();
    void Load();

    unsigned int GetVerticesCount() {
        return mVertices.size();
    }

    unsigned int GetIndicesCount() {
        return mIndices.size();
    }

    bool GetLoadStatus() {
        return mDoneLoading;
    }

private:
    unsigned int mVAO;
    
    std::vector<Mesh3D::Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    bool mDoneLoading = false;
};