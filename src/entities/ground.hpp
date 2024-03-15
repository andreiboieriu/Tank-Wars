#pragma once

#include <graphics/model.hpp>
#include <graphics/light.hpp>
#include <utils/transform.hpp>

class Ground {

public:
    Ground();
    void InitInstances(int count);
    void Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition);

private:
    Light mLight;
    std::shared_ptr<Model> mModel;
    Transform3D mTransform{};

    int mInstanceCount;

};