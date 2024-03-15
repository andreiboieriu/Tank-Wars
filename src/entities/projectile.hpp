#pragma once

#include <glm/glm.hpp>
#include <utils/transform.hpp>
#include <utils/hitbox.hpp>
#include <vector>
#include <entities/particle.hpp>

struct Particle;

struct Projectile {
    Transform3D transform;
    glm::vec3 velocity;
    HitBox hitBox;
    std::vector<Particle> particles;

    Projectile(Transform3D transform, glm::vec3 velocity, std::vector<Particle> particles, int tankHitBoxId) :
                transform{transform},
                velocity{velocity},
                particles{particles} {
        hitBox.center = transform.position;
        hitBox.rotationY = 0.f;
        hitBox.lengthOX = .1f;
        hitBox.lengthOZ = .1f;
        hitBox.lengthOY = .1f;
        hitBox.id = tankHitBoxId;
    }
};