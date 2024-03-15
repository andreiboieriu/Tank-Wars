#pragma once

#include <entities/enemy.hpp>
#include <entities/projectile_manager.hpp>
#include <utils/hitbox.hpp>

#include <utils/random_generator.hpp>

#include <memory>
#include <vector>

class EnemyManager {

public:
    EnemyManager(std::shared_ptr<ProjectileManager> projectileManager, std::vector<HitBox> buildings, std::shared_ptr<Player> player) :
                 mProjectileManager{projectileManager},
                 mPlayer{player} {

        std::vector<glm::vec3> positions;

        while (positions.size() != 20) {
            // generate random position, making sure not to be inside a building
            glm::vec3 position{};

            position.x = gRandomGenerator->GenFloat(5.f, 45.f);

            if (gRandomGenerator->GenInt(0, 1) == 1)
                position.x *= -1.f;
            
            position.z = gRandomGenerator->GenFloat(5.f, 45.f);

            if (gRandomGenerator->GenInt(0, 1) == 1)
                position.z *= -1.f;

            // check if new position is too close to an already generated one
            bool tooClose = false;

            for (auto& pos : positions) {
                if (glm::distance(pos, position) <= 2.f) {
                    tooClose = true;
                    break;
                }
            }

            if (glm::distance(position, mPlayer->GetHitBox().center) < 10.f)
                continue;

            for (auto& building : buildings) {
                if (glm::distance(position, building.center) <= 3.f) {
                    tooClose = true;
                    break;
                }
            }

            if (!tooClose) {
                positions.push_back(position);
            }
        }

        for (auto& pos : positions) {
            mEnemies.push_back(std::make_shared<Enemy>(pos, mPlayer));
        }
    }

    void Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition);

    void Update(float deltaTime, const std::vector<HitBox>& buildingHitBoxes, const std::shared_ptr<Player>& player);

    std::vector<HitBox> GetHitBoxes();

    std::vector<std::shared_ptr<Enemy>>& GetEnemies() {
        return mEnemies;
    }

private:
    std::vector<std::shared_ptr<Enemy>> mEnemies;
    std::shared_ptr<ProjectileManager> mProjectileManager;
    std::shared_ptr<Player> mPlayer;
};