#include "sprite.hpp"

#include "shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#include "utils/constants.hpp"

#include <glad/glad.h>

// std
#include <iostream>

Sprite::Sprite(const std::shared_ptr<Mesh2D>& mesh2D, const std::shared_ptr<Texture>& texture) :
               mMesh2D{mesh2D}, mTexture{texture} {
}

void Sprite::Render(const std::shared_ptr<Shader>& shader, const Transform2D& transform, 
                    glm::vec3 color, float alpha) {
    glStencilMask(0x00);
    shader->Use();

    glm::mat4 projection = glm::ortho(0.0f, LOGIC_WIDTH, 0.0f, LOGIC_HEIGHT, Z_NEAR, Z_FAR);

    glm::mat4 model(1.f);

    model = glm::translate(model, transform.position);

    model = glm::translate(model, transform.pivot);
    model = glm::rotate(model, transform.rotation, glm::vec3(0, 0, 1.f));

    model = glm::translate(model, transform.pivot * (-1.f));

    model = glm::scale(model, glm::vec3(transform.scale, 1.f));

    shader->SetUniform("model", model);
    shader->SetUniform("projection", projection);
    shader->SetUniform("spriteColor", color);
    shader->SetUniform("opacity", alpha);

    glActiveTexture(GL_TEXTURE0);
    mTexture->Bind();

    mMesh2D->BindVAO();

    glDrawElements(GL_TRIANGLES, mMesh2D->GetIndicesCount(), GL_UNSIGNED_INT, 0);

    mTexture->Unbind();
    mMesh2D->UnbindVAO();
}

