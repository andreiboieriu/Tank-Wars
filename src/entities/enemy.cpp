#include <entities/enemy.hpp>

Enemy::Enemy(glm::vec3 position, std::shared_ptr<Player> player) {
    mTank = std::make_unique<Tank>(position);
    mPlayer = player;
}

void Enemy::Update(float deltaTime, const std::vector<HitBox>& buildingsHitBox, std::shared_ptr<ProjectileManager>& projectileManager, const std::vector<HitBox>& tankHitBoxes) {
    if (mState != State::FOLLOW_PLAYER)
        mCurrentActionTimeLeft -= deltaTime;

    if (mCanFire == false) {
        mTimeFromLastFire += deltaTime;

        if (mTimeFromLastFire >= mFireCooldown) {
            mCanFire = true;
        }
    }

    if (mState != State::FOLLOW_PLAYER && mCurrentActionTimeLeft <= 0.f) {
        ChangeAction();
    }

    // tank is stuck
    if (mIsStuck) {
        mTimeStuck += deltaTime;

        if (mTimeStuck < 1.f)
            return;

        // do opposite action
        switch (mState) {
        case State::IDLE:
            break;

        case State::MOVE_FORWARD:
            mState = State::MOVE_BACKWARD;
            mCurrentActionTimeLeft = gRandomGenerator->GenFloat(1.f, 2.f);
            break;

        case State::ROTATE_LEFT:
            mState = State::ROTATE_RIGHT;
            mCurrentActionTimeLeft = gRandomGenerator->GenFloat(1.f, 2.5f);
            break;

        case State::ROTATE_RIGHT:
            mState = State::ROTATE_LEFT;
            mCurrentActionTimeLeft = gRandomGenerator->GenFloat(1.f, 2.5f);
            break;

        case State::MOVE_BACKWARD:
            mState = State::MOVE_FORWARD;
            mCurrentActionTimeLeft = gRandomGenerator->GenFloat(3.f, 7.f);
            break;

        case State::FOLLOW_PLAYER:
            break;
        }

        mIsStuck = false;
        mTimeStuck = 0.f;
    } else {
        if (mState != State::FOLLOW_PLAYER && glm::distance(GetHitBox().center, mPlayer->GetHitBox().center) < 9.f) {
            mLastState = mState;
            mState = State::FOLLOW_PLAYER;

            mTargetGunRotX = gRandomGenerator->GenFloat(-18.0f, 0.0f);
        }

        // reset turret rotation
        if (mLastState == State::FOLLOW_PLAYER) {
            float diff = mTargetTurretRotY - mTank->GetTurretRotY();
            mTank->RotateYTurret(diff * deltaTime);
        }

        bool closeToPlayer = false;

        if (glm::distance(GetHitBox().center, mPlayer->GetHitBox().center) < 13.f)
            closeToPlayer = true;

        switch (mState) {
        case State::IDLE:
            break;

        case State::MOVE_FORWARD:
            mTank->RotateXGun(deltaTime * 10.f);
            mIsStuck = (mTank->Move(deltaTime, buildingsHitBox, tankHitBoxes, closeToPlayer) == 0);
            break;

        case State::ROTATE_LEFT:
            mTank->RotateXGun(deltaTime * 10.f);
            mIsStuck = (mTank->RotateYBody(deltaTime, buildingsHitBox, tankHitBoxes) == 0);
            break;

        case State::ROTATE_RIGHT:
            mTank->RotateXGun(deltaTime * 10.f);
            mIsStuck = (mTank->RotateYBody(-deltaTime, buildingsHitBox, tankHitBoxes) == 0);
            break;

        case State::MOVE_BACKWARD:
            mTank->RotateXGun(deltaTime * 10.f);
            mIsStuck = (mTank->Move(-deltaTime, buildingsHitBox, tankHitBoxes, closeToPlayer) == 0);
            break;

        case State::FOLLOW_PLAYER:
            FacePlayer(deltaTime, buildingsHitBox, tankHitBoxes, projectileManager);
            break;
        }
    }
}

void Enemy::ChangeAction() {
    if (mState == State::MOVE_BACKWARD || mState == State::MOVE_FORWARD) {
        int rand = gRandomGenerator->GenInt(0, 1);

        if (rand == 0) {
            mState = State::ROTATE_LEFT;
        } else {
            mState = State::ROTATE_RIGHT;
        }

        mCurrentActionTimeLeft = gRandomGenerator->GenFloat(1.f, 2.5f);
        return;
    }

    mState = State::MOVE_FORWARD;

    mCurrentActionTimeLeft = gRandomGenerator->GenFloat(3.f, 7.f);
}

bool Enemy::IsStuck() {
    return true;
}

void Enemy::Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition) {
    if (mIsTargeted) {
        mTank->RenderStencil(viewMatrix, cameraPosition);
    } else {
        mTank->Render(viewMatrix, cameraPosition);
    }

    mIsTargeted = false;
}

HitBox Enemy::GetHitBox() {
    return mTank->GetHitBox();
}

void Enemy::TakeHit() {
    mTank->DecrementHp();
}

bool Enemy::IsDead() {
    return (mTank->GetHp() == 0);
}

void Enemy::FacePlayer(float deltaTime, const std::vector<HitBox>& buildingsHitBox, const std::vector<HitBox>& tankHitBoxes, std::shared_ptr<ProjectileManager>& projectileManager) {
    if (glm::distance(GetHitBox().center, mPlayer->GetHitBox().center) > 12.f) {
        mState = mLastState;
        mLastState = State::FOLLOW_PLAYER;
        return;
    }

    glm::vec3 direction = glm::normalize(mPlayer->GetHitBox().center - GetHitBox().center);

    float targetAngle = std::atan2(direction.x, direction.z);
    float angle = std::atan2(mTank->GetGunDirection().x, mTank->GetGunDirection().z);

    float diff = mTargetGunRotX - mTank->GetGunRotX();
    mTank->RotateXGun(deltaTime * diff);

    diff = glm::degrees(targetAngle - angle);

    if (glm::abs(360.f - diff) < glm::abs(diff)) {
        diff -= 360.f;
    }

    if (diff > 360.f)
        diff -= 360.f;

    if (diff < -360.f)
        diff += 360.f;

    // turn turret towards player
    mTank->RotateYTurret(deltaTime * diff);

    if (glm::abs(diff) < 1.f && mCanFire) {
        mTank->Fire(projectileManager);
        mCanFire = false;
        mTimeFromLastFire = 0.f;

        mTargetGunRotX = gRandomGenerator->GenFloat(-18.0f, 0.0f);
    }
}
