#pragma once

#include <glm/glm.hpp>

struct Light {
    glm::vec3 direction;
    glm::vec3 diffuseStrength;
    glm::vec3 ambientStrength;
    glm::vec3 specularStrength;
};