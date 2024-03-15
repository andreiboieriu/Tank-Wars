#include <entities/building_manager.hpp>
#include <utils/resource_manager.hpp>
#include <utils/random_generator.hpp>

BuildingManager::BuildingManager(int count) {
    mModel = gResourceManager->GetModel("building");

    mInstanceCount = count * count;
    InitTransforms(count);

    mLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    mLight.ambientStrength = glm::vec3(0.2f, 0.2f, 0.2f);
    mLight.diffuseStrength = glm::vec3(0.4f, 0.4f, 0.4f);
    mLight.specularStrength = glm::vec3(0.7f, 0.7f, 0.7f);
}

void BuildingManager::InitTransforms(unsigned int count) {
    while (mHitboxes.size() != count) {
        Transform3D newTransform{};
        newTransform.position.y = -0.3f;
        newTransform.position.x = gRandomGenerator->GenFloat(6.0f, 45.0f);

        if (gRandomGenerator->GenInt(0, 1)) {
            newTransform.position.x *= -1.f;
        }

        newTransform.position.z = gRandomGenerator->GenFloat(6.0f, 45.0f);

        if (gRandomGenerator->GenInt(0, 1)) {
            newTransform.position.z *= -1.f;
        }

        newTransform.rotation.y = gRandomGenerator->GenFloat(0.f, 360.f);
        newTransform.scale = glm::vec3(.16f);

        HitBox newHitBox = {newTransform.position,
                            newTransform.rotation.y,
                            16.f * .16f,
                            10.f * .16f,
                            12.f * .16f};

        bool tooClose = false;

        for (auto& hitBox : mHitboxes) {
            if (glm::distance(hitBox.center, newHitBox.center) < 5.f) {
                tooClose = true;
                break;
            }
        }

        if (tooClose)
            continue;

        mTransforms.push_back(newTransform);

        // add hitbox to hitbox vector
        mHitboxes.push_back(newHitBox);
    }

    // add main menu building
    Transform3D newTransform{};
    newTransform.position = glm::vec3(-1.5f, -0.3f, 0.0f);
    newTransform.scale = glm::vec3(.16f);
    newTransform.rotation.y = -90.f;

    HitBox newHitBox = {newTransform.position,
                            newTransform.rotation.y,
                            16.f * .16f,
                            10.f * .16f,
                            12.f * .16f};
    mTransforms.push_back(newTransform);

        // add hitbox to hitbox vector
    mHitboxes.push_back(newHitBox);
}

void BuildingManager::Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition) {
    std::shared_ptr<Shader> shader = gResourceManager->GetShader("model_shader");

    for (auto& transform : mTransforms) {
        mModel->Render(shader, viewMatrix, transform, cameraPosition, mLight, 1.0f);
    }
}