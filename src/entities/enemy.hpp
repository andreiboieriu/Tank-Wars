#pragma once

#include <entities/projectile_manager.hpp>
#include <entities/tank.hpp>
#include <entities/player.hpp>
#include <utils/hitbox.hpp>
#include <glm/glm.hpp>

#include <memory>

class ProjectileManager;
class Tank;
class Player;

class Enemy {

public:
    Enemy(glm::vec3 position, std::shared_ptr<Player> player);
    void Update(float deltaTime, const std::vector<HitBox>& buildingsHitBox, std::shared_ptr<ProjectileManager>& projectileManager, const std::vector<HitBox>& tankHitBoxes);
    void Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition);

    HitBox GetHitBox();

    void SetTargetedFlag() {
        mIsTargeted = true;
    }

    bool IsTargeted() {
        return mIsTargeted;
    }

    void TakeHit();

    bool IsDead();
private:
    void ChangeAction();
    bool IsStuck();
    void FacePlayer(float deltaTime, const std::vector<HitBox>& buildingsHitBox, const std::vector<HitBox>& tankHitBoxes, std::shared_ptr<ProjectileManager>& projectileManager);

    glm::vec3 mLastPosition{};
    float mLastRotY = 0.f;

    std::unique_ptr<Tank> mTank;
    std::shared_ptr<Player> mPlayer;

    float mCurrentActionTimeLeft = 0.f;
    bool mIsTargeted = false;

    float mTimeStuck = 0.f;

    enum class State {
        IDLE,
        MOVE_FORWARD,
        MOVE_BACKWARD,
        ROTATE_LEFT,
        ROTATE_RIGHT,
        FOLLOW_PLAYER
    };

    const float mRotateDuration = 1.f;
    float mMoveDuration = 2.f;
    State mState;

    State mLastState = State::IDLE;

    bool mIsStuck = false;

    bool mCanFire = true;
    float mTimeFromLastFire = 0.f;
    float mFireCooldown = 1.5f;

    float mTargetGunRotX = 0.f;
    float mTargetTurretRotY = 0.f;
};