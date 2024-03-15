#include <entities/enemy_manager.hpp>

void EnemyManager::Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition) {
    std::shared_ptr<Enemy> targetedEnemy = nullptr;

    for (auto& enemy : mEnemies) {
        if (!enemy->IsTargeted())
            enemy->Render(viewMatrix, cameraPosition);
        else
            targetedEnemy = enemy;
    }

    if (targetedEnemy != nullptr)
        targetedEnemy->Render(viewMatrix, cameraPosition);
}

void EnemyManager::Update(float deltaTime, const std::vector<HitBox>& buildingHitBoxes, const std::shared_ptr<Player>& player) {
    auto hitBoxes = GetHitBoxes();
    hitBoxes.push_back(player->GetHitBox());

    for (auto& enemy : mEnemies) {
        if (enemy->IsDead())
            continue;
            
        enemy->Update(deltaTime, buildingHitBoxes, mProjectileManager, hitBoxes);
    }
}

std::vector<HitBox> EnemyManager::GetHitBoxes() {
    std::vector<HitBox> hitBoxes;

    for (auto& enemy : mEnemies) {
        hitBoxes.push_back(enemy->GetHitBox());
    }

    return hitBoxes;
}