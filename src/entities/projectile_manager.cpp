#include <entities/projectile_manager.hpp>
#include <utils/resource_manager.hpp>
#include <graphics/shader.hpp>

ProjectileManager::ProjectileManager() {
    mModel = gResourceManager->GetModel("tank_projectile");
    mParticleModel = gResourceManager->GetModel("cube");

    mLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    mLight.ambientStrength = glm::vec3(0.2f, 0.2f, 0.2f);
    mLight.diffuseStrength = glm::vec3(0.4f, 0.4f, 0.4f);
    mLight.specularStrength = glm::vec3(0.7f, 0.7f, 0.7f);
}


void ProjectileManager::Update(float deltaTime, const std::vector<HitBox>& buildingsHitBox, std::vector<std::shared_ptr<Enemy>>& enemies, std::shared_ptr<Player>& player, int& score) {
    auto it = mProjectiles.begin();

    while (it != mProjectiles.end()) {
        Projectile* projectile = *it;

        bool hit = false;

        projectile->velocity.y -= mGravity * deltaTime;
        projectile->transform.position += projectile->velocity * deltaTime;

        projectile->hitBox.center = projectile->transform.GetWorldPosition();

        for (auto& hitBox : buildingsHitBox) {
            if (hitBox.id == projectile->hitBox.id)
                continue;

            if (projectile->hitBox.CheckCollision(hitBox)) {
                Explode(projectile);
                delete projectile;
                it = mProjectiles.erase(it);
                hit = true;
                break;
            }
        }

        if (hit) {
            continue;
        }

        for (auto& enemy : enemies) {
            HitBox hitBox = enemy->GetHitBox();

            if (hitBox.id == projectile->hitBox.id)
                continue;

            if (projectile->hitBox.CheckCollision(hitBox)) {
                enemy->TakeHit();

                score += 50;

                Explode(projectile);
                delete projectile;
                it = mProjectiles.erase(it);
                hit = true;
                break;
            }
        }

        if (hit) {
            continue;
        }

        HitBox hitBox = player->GetHitBox();

        if (hitBox.id != projectile->hitBox.id && projectile->hitBox.CheckCollision(hitBox)) {
            Explode(projectile);
            delete projectile;
            it = mProjectiles.erase(it);

            player->TakeHit();
            continue;
        }

        if (projectile->transform.position.y <= -0.2f) {
            Explode(projectile);
            delete projectile;
            it = mProjectiles.erase(it);
            continue;
        }

        double angle = glm::degrees(-glm::asin(glm::normalize(projectile->velocity).y));
        projectile->transform.rotation.x = angle;

        auto particleIt = projectile->particles.begin();

        while (particleIt != projectile->particles.end()) {
            particleIt->Update(deltaTime, -glm::normalize(projectile->velocity));

            if (particleIt->lifeSpan <= 0.f) {
                particleIt->Reset();
            }

            particleIt++;
        }

        it++;
    }

    auto expIt = mExplosions.begin();

    while (expIt != mExplosions.end()) {
        Explosion *exp = *expIt;

        exp->Update(deltaTime);

        if (exp->finished) {
            delete exp;

            expIt = mExplosions.erase(expIt);
            continue;
        }

        expIt++;
    }
}

void ProjectileManager::Explode(Projectile *projectile) {
    mExplosions.push_back(new Explosion(projectile));
}

void ProjectileManager::AddProjectile(glm::vec3 direction, Transform3D transform, int tankHitBoxId) {
    std::vector<Particle> particles;

    for (int i = 0; i < 400; i++) {
        particles.push_back(Particle(-direction, true, 15.f, .7f));
    }

    mProjectiles.push_back(new Projectile(transform, direction * mProjectileSpeed, particles, tankHitBoxId));
}

void ProjectileManager::Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition) {
    std::shared_ptr<Shader> shader = gResourceManager->GetShader("model_shader");
    std::shared_ptr<Shader> particleShader = gResourceManager->GetShader("particle_shader");

    for (auto& projectile : mProjectiles) {
        mModel->Render(shader, viewMatrix, projectile->transform, cameraPosition, mLight, 1.0f);
        Transform3D transform = projectile->transform;
        transform.scale = glm::vec3(0.002f);

        mParticleModel->RenderInstancedParticles(particleShader, viewMatrix, transform, cameraPosition, mLight, projectile->particles);
    }

    for (auto& explosion : mExplosions) {
        mParticleModel->RenderInstancedParticles(particleShader, viewMatrix, explosion->transform, cameraPosition, mLight, explosion->particles);
    }
}

void ProjectileManager::RenderTrajectory(glm::vec3 direction, Transform3D transform, glm::mat4 viewMatrix, glm::vec3 cameraPosition, std::vector<HitBox> tankHitBoxes, std::vector<HitBox> buildingHitBoxes) {
    glm::vec3 velocity = direction * mProjectileSpeed;

    transform.scale = glm::vec3(0.03f, 0.0001f, 0.01f);

    HitBox hitBox{};
    hitBox.center = transform.position;
    hitBox.lengthOX = .1f;
    hitBox.lengthOZ = .1f;
    hitBox.lengthOY = .1f;

    auto cubeModel = gResourceManager->GetModel("cube");
    auto shader = gResourceManager->GetShader("trajectory_shader");

    float stepTime = 1.f / 480.f;

    while (transform.position.y > -0.3f) {
        velocity.y -= mGravity * stepTime;
        transform.position += velocity * stepTime;

        double angle = glm::degrees(-glm::asin(glm::normalize(velocity).y));
        transform.rotation.x = angle;
        hitBox.center = transform.GetWorldPosition();

        cubeModel->Render(shader, viewMatrix, transform, cameraPosition, mLight, 0.5f);

        for (unsigned int i = 0; i < tankHitBoxes.size(); i++) {
            if (hitBox.CheckCollision(tankHitBoxes[i])) {
                return;
            }
        }

        for (unsigned int i = 0; i < buildingHitBoxes.size(); i++) {
            if (hitBox.CheckCollision(buildingHitBoxes[i])) {
                return;
            }
        }

    }
}

int ProjectileManager::SimulateTrajectory(glm::vec3 direction, Transform3D transform, std::vector<HitBox> tankHitBoxes) {
    glm::vec3 velocity = direction * mProjectileSpeed;

    transform.scale = glm::vec3(0.1f);

    HitBox hitBox{};
    hitBox.center = transform.position;
    hitBox.lengthOX = .1f;
    hitBox.lengthOZ = .1f;
    hitBox.lengthOY = .1f;

    auto cubeModel = gResourceManager->GetModel("cube");
    auto shader = gResourceManager->GetShader("model_shader");

    float stepTime = 1.f / 60.f;

    while (transform.position.y > -0.3f) {
        velocity.y -= mGravity * stepTime;
        transform.position += velocity * stepTime;
        double angle = glm::degrees(-glm::asin(glm::normalize(velocity).y));
        transform.rotation.x = angle;
        hitBox.center = transform.GetWorldPosition();
        
        for (unsigned int i = 0; i < tankHitBoxes.size(); i++) {
            if (hitBox.CheckCollision(tankHitBoxes[i])) {
                return i;
            }
        }
    }

    return -1;
}
