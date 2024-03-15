#pragma once

#include <graphics/model.hpp>
#include <graphics/light.hpp>
#include <utils/hitbox.hpp>

#include <glm/glm.hpp>

#include <memory>

class BuildingManager {

public:
    BuildingManager(int count);

    void Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition);
    
    std::vector<HitBox> GetHitBoxes() {
        return mHitboxes;
    }

private:
    void InitTransforms(unsigned int count);

    int mInstanceCount;

    Light mLight{};
    std::shared_ptr<Model> mModel;
    std::vector<HitBox> mHitboxes;

    std::vector<glm::mat4> modelMatrices;
    std::vector<Transform3D> mTransforms;
};
