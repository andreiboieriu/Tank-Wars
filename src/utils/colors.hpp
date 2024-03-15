#pragma once

#include "glm/glm.hpp"

// std
#include <vector>

namespace colors {

extern glm::vec3 green;
extern glm::vec3 blue;
extern glm::vec3 red;
extern glm::vec3 orange;
extern glm::vec3 salmon;
extern glm::vec3 yellow;
extern glm::vec3 turqoise;
extern glm::vec3 magenta;
extern glm::vec3 gold;
extern glm::vec3 grey;
extern glm::vec3 ruby;
extern glm::vec3 diamond;
extern glm::vec3 emerald;
extern glm::vec3 lightBrown;
extern glm::vec3 darkGrey;
extern glm::vec3 cadmiumViolet;

extern std::vector<glm::vec3> GameColorVec;

bool ColorEqual(glm::vec3 c1, glm::vec3 c2);

} // namespace colors;