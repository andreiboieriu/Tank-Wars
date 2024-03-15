#include <entities/ground.hpp>

#include <utils/resource_manager.hpp>
#include <graphics/shader.hpp>
#include <glad/glad.h>
#include <memory>

Ground::Ground() {
    mModel = gResourceManager->GetModel("ground_tile");
    mTransform.scale = glm::vec3(1.f);
    mTransform.position = glm::vec3(0.f, -0.3f, 0.f);

    mLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    mLight.ambientStrength = glm::vec3(0.2f, 0.2f, 0.2f);
    mLight.diffuseStrength = glm::vec3(0.4f, 0.4f, 0.4f);
    mLight.specularStrength = glm::vec3(0.7f, 0.7f, 0.7f);

    InitInstances(130);
}

void Ground::InitInstances(int count) {
    std::vector<glm::vec3> translations;

    for (int i = -count / 2; i <= count / 2; i++) {
        for (int j = - count / 2; j <= count / 2; j++) {
            translations.push_back(glm::vec3(i, 0, j));
        }
    }

    mInstanceCount = translations.size();
    std::cout << "Ground instances count: " << mInstanceCount << std::endl;

    // store translations in a VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(translations[0]) * translations.size(), translations.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // this model only has one mesh
    mModel->getMeshVec()[0]->BindVAO();

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(4, 1);

    mModel->getMeshVec()[0]->UnbindVAO();
}

void Ground::Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition) {
    std::shared_ptr<Shader> shader = gResourceManager->GetShader("ground_shader");
    mModel->RenderInstanced(shader, viewMatrix, mTransform, cameraPosition, mLight, mInstanceCount);
}