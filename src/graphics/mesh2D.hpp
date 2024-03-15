#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// std
#include <vector>
#include <memory>

class Mesh2D {

public:
    struct Vertex {
        glm::vec2 position;
        glm::vec2 texCoords;

        Vertex(glm::vec2 position, glm::vec2 texCoords);
    };

    Mesh2D() {}

    Mesh2D(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh2D();

    void BindVAO();
    void UnbindVAO();

    GLuint GetVerticesCount() {
        return mVerticesCount;
    }

    GLuint GetIndicesCount() {
        return mIndicesCount;
    }

    static std::shared_ptr<Mesh2D> CreateSquare();

private:
    static void InsertIndices(char a, char b, char c, std::vector<unsigned int>& indices) {
        indices.push_back(a - 'a');
        indices.push_back(b - 'a');
        indices.push_back(c - 'a');
    }

    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;

    GLuint mVAO;
    GLuint mVerticesCount;
    GLuint mIndicesCount;
};