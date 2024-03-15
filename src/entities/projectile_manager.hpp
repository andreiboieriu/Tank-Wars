#pragma once

#include <entities/player.hpp>

#include <graphics/model.hpp>
#include <utils/transform.hpp>
#include <memory>
#include <vector>
#include <graphics/light.hpp>

#include <entities/particle.hpp>
#include <entities/projectile.hpp>
#include <utils/hitbox.hpp>

class Enemy;
class Player;

class ProjectileManager {

public:
    ProjectileManager();
    void Update(float deltaTime, const std::vector<HitBox>& buildingsHitBox, std::vector<std::shared_ptr<Enemy>>& enemies, std::shared_ptr<Player>& player, int& score);
    void AddProjectile(glm::vec3 direction, Transform3D transform, int tankHitBoxId);
    void Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition);

    void RenderTrajectory(glm::vec3 direction, Transform3D transform, glm::mat4 viewMatrix, glm::vec3 cameraPosition, std::vector<HitBox> tankHitBoxes, std::vector<HitBox> buildingHitBoxes);
    int SimulateTrajectory(glm::vec3 direction, Transform3D transform, std::vector<HitBox> tankHitBoxes);

private:
    void Explode(Projectile *projectile);

    std::shared_ptr<Model> mModel;
    std::shared_ptr<Model> mParticleModel;
    std::vector<Projectile *> mProjectiles;

    std::vector<Explosion *> mExplosions;
    Light mLight;

    const float mProjectileSpeed = 10.f;
    const float mGravity = 5.0f;
};