#pragma once

#include "glm/glm.hpp"
#include "graphics/shader.hpp"
#include "mesh2D.hpp"
#include "utils/transform.hpp"
#include "texture.hpp"

// std
#include <string>
#include <vector>
#include <memory>

class Sprite {
public:
    Sprite() {}
    ~Sprite() {}
    Sprite(const std::shared_ptr<Mesh2D>& mesh2D, const std::shared_ptr<Texture>& texture);


    void Render(const std::shared_ptr<Shader>& shader, const Transform2D& transform, 
                glm::vec3 color, float alpha);

private:
    std::shared_ptr<Mesh2D> mMesh2D;
    std::shared_ptr<Texture> mTexture;

};