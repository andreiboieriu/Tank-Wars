#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform2D {
    glm::vec3 position;
    float rotation;
    glm::vec3 pivot;
    glm::vec2 scale;
};

struct Transform3D {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::vec3 rotationPivotY;
    glm::vec3 rotationPivotX;

    glm::mat4 GetModelMatrix() {
        glm::mat4 model(1.f);

        model = glm::translate(model, position);    

        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::translate(model, -rotationPivotY);
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, rotationPivotY);    

        model = glm::translate(model, -rotationPivotX);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, rotationPivotX);

        model = glm::scale(model, scale);

        return model;
    }

    glm::vec3 GetWorldPosition() {
        return glm::vec3(GetModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
};

