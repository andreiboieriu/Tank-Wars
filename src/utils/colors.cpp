#include "colors.hpp"

#include "glm/glm.hpp"

// std
#include <vector>

namespace colors {

glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 orange = glm::vec3(1.0f, 0.5f, 0.0f);
glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 turqoise = glm::vec3(0.5f, 1.0f, 1.0f);
glm::vec3 magenta = glm::vec3(1.f, 0.5f, 1.f);
glm::vec3 salmon = glm::vec3(1.f, 0.5f, 0.5f);
glm::vec3 gold = glm::vec3(1.f, 0.87f, 0.0f);
glm::vec3 cadmiumViolet = glm::vec3(.498f, .2431f, .5961f);
glm::vec3 grey = glm::vec3(.5f, .5f, .5f);
glm::vec3 darkGrey = glm::vec3(.2f, .2f, .2f);
glm::vec3 lightBrown = glm::vec3(.6471f, .4471f, .3216f);
glm::vec3 ruby = glm::vec3(0.9804f, 0.0f, 0.349f);
glm::vec3 diamond = glm::vec3(0.6902f, 0.9686f, 1.0f);
glm::vec3 emerald = glm::vec3(0.2549f, 0.8431f, 0.451f);

std::vector<glm::vec3> GameColorVec = {
    green, ruby, magenta, turqoise
};

bool ColorEqual(glm::vec3 c1, glm::vec3 c2) {
    if (c1.x != c2.x)
        return false;

    if (c1.y != c2.y)
        return false;

    if (c1.z != c2.z)
        return false;

    return true;
}

} // namespace colors;