#include <entities/player.hpp>
#include <fstream>
#include <sstream>

#include <GLFW/glfw3.h>

Player::Player(Transform3D transform, std::shared_ptr<ProjectileManager> projectileManager) {
    mTank = std::make_unique<Tank>(transform.position);
    mProjectileManager = projectileManager;

    mCamera = std::make_unique<Camera>();
    UpdateCamera(0.0f);
}

void Player::SetModels() {

}

void Player::Update(float deltaTime, std::vector<HitBox> buildingsHitBox, std::vector<HitBox> enemyHitBoxes, std::vector<std::shared_ptr<Enemy>>& enemies) {
    if (mWKeyHold) {
        mTank->Move(deltaTime, buildingsHitBox, enemyHitBoxes, true);
    }

    if (mSKeyHold) {
        mTank->Move(-deltaTime, buildingsHitBox, enemyHitBoxes, true);
    }

    if (mAKeyHold) {
       mTank->RotateYBody(deltaTime, buildingsHitBox, enemyHitBoxes);
    }

    if (mDKeyHold) {
       mTank->RotateYBody(-deltaTime, buildingsHitBox, enemyHitBoxes);
    }

    if (mMouseMoved && !mRightMouseButtonHold) {
        float xOffset = mCurrentMouseCoords.x - mLastMouseCoords.x;
        float yOffset = mCurrentMouseCoords.y - mLastMouseCoords.y;

        xOffset *= -mTurretSensitivity * deltaTime;
        yOffset *= -mGunSensitivity * deltaTime;

        mTank->RotateYTurret(xOffset);
        mTank->RotateXGun(yOffset);

        mMouseMoved = false;
    }

    UpdateCamera(deltaTime);

    int res = mProjectileManager->SimulateTrajectory(mTank->GetGunDirection(),
                                           mTank->GetProjectileTransform(),
                                           enemyHitBoxes);

    if (res != -1) {
        enemies[res]->SetTargetedFlag();
    }

    if (mCanFire == false) {
        mTimeFromLastFire += deltaTime;

        if (mTimeFromLastFire >= mFireCooldown) {
            mCanFire = true;
        }
    }
}

void Player::UpdateCamera(float deltaTime) {
    // update camera position
    mCamera->SetPosition(mTank->GetCameraPosition());

    // update camera direction
    mCamera->SetDirection(mTank->GetCameraDirection());

    // camera free view
    if (mRightMouseButtonHold) {
        mCamera->FreeView(deltaTime, mFirstFreeViewFrame);

        if (mFirstFreeViewFrame == true)
            mFirstFreeViewFrame = false;
    } else {
        mFirstFreeViewFrame = true;
    }

    // update view matrix
    mCamera->UpdateViewMatrix();
}

void Player::Render(std::vector<HitBox> tankHitBoxes, std::vector<HitBox> buildingHitBoxes) {
    glm::mat4 viewMatrix = mCamera->GetViewMatrix();
    glm::vec3 cameraPosition = mCamera->GetPosition();

    mTank->Render(viewMatrix, cameraPosition);

    if (mShouldRenderTrajectory)
        mProjectileManager->RenderTrajectory(mTank->GetGunDirection(), mTank->GetProjectileTransform(), viewMatrix, cameraPosition, tankHitBoxes, buildingHitBoxes);
}

void Player::OnMouseBtnPress(int button, int action, int mods) {
    // ignore repeat actions
    if (action == GLFW_REPEAT)
        return;

    bool keyState = (action == GLFW_PRESS);

    if (keyState && button == GLFW_MOUSE_BUTTON_LEFT) {
        if (mCanFire) {
            mTank->Fire(mProjectileManager);
            mCanFire = false;
            mTimeFromLastFire = 0.f;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        mRightMouseButtonHold = keyState;
    }
}

void Player::OnMouseMove(glm::vec2 mouseCoords) {
    mCamera->OnMouseMove(mouseCoords);

    mLastMouseCoords = mCurrentMouseCoords;
    mCurrentMouseCoords = mouseCoords;

    if (mFirstTimeMovingMouse) {
        mFirstTimeMovingMouse = false;
        return;
    }

    mMouseMoved = true;
}

void Player::OnKeyPressed(int key, int scancode, int action, int mode) {
    // ignore repeat actions
    if (action == GLFW_REPEAT)
        return;

    // true if key is pressed, false if key is released
    bool keyState = (action == GLFW_PRESS);

    if (keyState) {
        if (key == GLFW_KEY_SPACE) {
            mShouldRenderTrajectory = !mShouldRenderTrajectory;
        }
    }

    switch (key) {
    case GLFW_KEY_W:
        mWKeyHold = keyState;
        break;

    case GLFW_KEY_S:
        mSKeyHold = keyState;
        break;

    case GLFW_KEY_D:
        mDKeyHold = keyState;
        break;

    case GLFW_KEY_A:
        mAKeyHold = keyState;
        break;
    }
}

HitBox Player::GetHitBox() {
    return mTank->GetHitBox();
}

void Player::TakeHit() {
    mTank->DecrementHp();
}

bool Player::IsDead() {
    return (mTank->GetHp() == 0);
}
