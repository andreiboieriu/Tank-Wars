#include <graphics/mesh3D.hpp>
#include <glad/glad.h>

// std
#include <memory>
#include <vector>

Mesh3D::Mesh3D(const std::vector<Vertex>& vertices,
               const std::vector<unsigned int>& indices,
               bool buffer) {
    mVertices = vertices;
    mIndices = indices;

    if (buffer)
        return;

    Load();
}

void Mesh3D::Load() {
    // gen and bind VAO
    glGenVertexArrays(1, &mVAO);
    BindVAO();

    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertices[0]) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndices[0]) * mIndices.size(), mIndices.data(), GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Mesh3D::Vertex),
                          (void *)0);

    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Mesh3D::Vertex),
                          (void *)offsetof(Mesh3D::Vertex, normal));

    // texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Mesh3D::Vertex),
                          (void *)offsetof(Mesh3D::Vertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Mesh3D::Vertex),
                          (void *)offsetof(Mesh3D::Vertex, tangent));

    // unbind VAO
    UnbindVAO();

    mDoneLoading = true;
}

Mesh3D::~Mesh3D() {
    glDeleteVertexArrays(1, &mVAO);
}

void Mesh3D::BindVAO() {
    glBindVertexArray(mVAO);
}

void Mesh3D::UnbindVAO() {
    glBindVertexArray(0);
}

// Mesh3D::Vertex(glm::vec2 position, glm::vec2 texCoords) {
//     this->position = position;
//     this->texCoords = texCoords;
// }



