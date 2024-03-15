#pragma once

#include <graphics/model.hpp>
#include <utils/transform.hpp>
#include <utils/resource_manager.hpp>
#include <core/camera.hpp>
#include <entities/tank.hpp>
#include <entities/projectile_manager.hpp>
#include <entities/enemy.hpp>

#include <memory>

class Enemy;

class Player {

public:
    Player(Transform3D transform, std::shared_ptr<ProjectileManager> projectileManager);
    void Update(float deltaTime, std::vector<HitBox> buildingsHitBox, std::vector<HitBox> enemyHitBoxes, std::vector<std::shared_ptr<Enemy>>& enemies);
    void Render(std::vector<HitBox> tankHitBoxes, std::vector<HitBox> buildingHitBoxes);

    void OnMouseBtnPress(int button, int action, int mods);
    void OnMouseMove(glm::vec2 mouseCoords);
    void OnKeyPressed(int key, int scancode, int action, int mode);

    glm::mat4 GetViewMatrix() {
        return mCamera->GetViewMatrix();
    }

    glm::vec3 GetCameraPosition() {
        return mCamera->GetPosition();
    }

    void SetCameraDirection(glm::vec3 direction) {
        mCamera->SetDirection(direction);
    }

    void SetCameraPosition(glm::vec3 position) {
        mCamera->SetPosition(position);
    }

    void DisableTrajectory() {
        mShouldRenderTrajectory = false;
    }

    bool IsDead();

    HitBox GetHitBox();

    void SetModels();

    void TakeHit();
private:
    void UpdateCamera(float deltaTime);

    std::unique_ptr<Tank> mTank;
    std::shared_ptr<ProjectileManager> mProjectileManager;

    glm::vec3 mCameraDirection;

    bool mWKeyHold = false;
    bool mDKeyHold = false;
    bool mAKeyHold = false;
    bool mSKeyHold = false;
    bool mRightMouseButtonHold = false;

    std::unique_ptr<Camera> mCamera;
    bool mFirstFreeViewFrame = true;

    glm::vec2 mLastMouseCoords;
    glm::vec2 mCurrentMouseCoords;
    bool mFirstTimeMovingMouse = true;
    bool mMouseMoved = false;

    float mTurretSensitivity = 20.f;
    float mGunSensitivity = 20.f;

    bool mShouldRenderTrajectory = false;

    bool mCanFire = true;
    float mTimeFromLastFire = 0.f;
    float mFireCooldown = 1.5f;
};