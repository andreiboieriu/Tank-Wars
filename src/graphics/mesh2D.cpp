#include <graphics/mesh2D.hpp>

// std
#include <memory>
#include <vector>

Mesh2D::Mesh2D(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    // gen and bind VAO
    glGenVertexArrays(1, &mVAO);
    BindVAO();

    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // unbind VAO
    UnbindVAO();

    mVerticesCount = vertices.size();
    mIndicesCount = indices.size();
}

Mesh2D::~Mesh2D() {
    glDeleteVertexArrays(1, &mVAO);
}

void Mesh2D::BindVAO() {
    glBindVertexArray(mVAO);
}

void Mesh2D::UnbindVAO() {
    glBindVertexArray(0);
}

Mesh2D::Vertex::Vertex(glm::vec2 position, glm::vec2 texCoords) {
    this->position = position;
    this->texCoords = texCoords;
}

std::shared_ptr<Mesh2D> Mesh2D::CreateSquare() {
    std::vector<Mesh2D::Vertex> vertices = {
        Mesh2D::Vertex(glm::vec2(-0.5f, 0.5f), glm::vec2(0.f, 1.f)),
        Mesh2D::Vertex(glm::vec2(-0.5f, -0.5f), glm::vec2(0.f, 0.f)),
        Mesh2D::Vertex(glm::vec2(0.5f, -0.5f), glm::vec2(1.f, 0.f)),
        Mesh2D::Vertex(glm::vec2(0.5f, 0.5f), glm::vec2(1.f, 1.f))
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    return std::make_shared<Mesh2D>(vertices, indices);
}



