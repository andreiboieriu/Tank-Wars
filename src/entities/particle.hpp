#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <utils/random_generator.hpp>
#include <entities/projectile.hpp>
#include <vector>

struct Particle {
    glm::vec3 offset;
    glm::vec3 direction;
    float speed;
    float initialLifeSpan;
    float lifeSpan;
    float opacity;

    bool reusable;

    float rotX;
    float rotY;

    float maxRot;
    float maxLifeSpan;

    Particle(glm::vec3 direction, bool reusable, float maxRot, float maxLifeSpan) :
                direction{direction},
                reusable{reusable},
                maxRot{maxRot},
                maxLifeSpan{maxLifeSpan} {
        Reset();
    }

    void Update(float deltaTime, glm::vec3 newDirection) {
        lifeSpan -= deltaTime;
        opacity = lifeSpan / initialLifeSpan;

        direction = newDirection;
        direction = glm::rotateX(direction, rotX);
        direction = glm::rotateY(direction, rotY);

        offset += direction * speed * deltaTime;
    }

    void Update(float deltaTime) {
        lifeSpan -= deltaTime;
        opacity = lifeSpan / initialLifeSpan;

        offset += direction * speed * deltaTime;
    }

    void Reset() {
        speed = gRandomGenerator->GenFloat(0.1f, .6f);

        offset = glm::vec3(0.f, 0.f, 0.f);

        initialLifeSpan = gRandomGenerator->GenFloat(.1f, maxLifeSpan);
        lifeSpan = initialLifeSpan;
        opacity = 1.f;

        rotX = glm::radians(gRandomGenerator->GenFloat(-maxRot, maxRot));
        rotY = glm::radians(gRandomGenerator->GenFloat(-maxRot, maxRot));

        direction = glm::rotateX(direction, rotX);
        direction = glm::rotateY(direction, rotY);
    }
};

struct Explosion {
    std::vector<Particle> particles;
    Transform3D transform{};
    glm::vec3 direction;

    bool finished = false;

    Explosion(Projectile *projectile) {
        glm::vec3 direction = -glm::normalize(projectile->velocity);

        for (int i = 0; i < 400; i++) {
            particles.push_back(Particle(direction, false, 50.f, 1.5f));
        }

        transform = projectile->transform;
        transform.scale = glm::vec3(0.007f);
    }

    void Update(float deltaTime) {
        auto it = particles.begin();

        while (it != particles.end()) {
            it->Update(deltaTime);

            if (it->lifeSpan <= 0.f) {
                it = particles.erase(it);
            } else {
                it++;
            }
        }

        if (particles.empty()) {
            finished = true;
        }
    }
};

