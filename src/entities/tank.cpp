#include <entities/tank.hpp>

#include <utils/resource_manager.hpp>

#include <glm/gtx/rotate_vector.hpp>


#include <fstream>
#include <sstream>

Tank::Tank(glm::vec3 position) {
    mTransform.position = position;
    mTransform.scale = glm::vec3(1.f);
    
    SetModels();
    SetRotPivots();
    InitTracks();

    mLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    mLight.ambientStrength = glm::vec3(0.5f, 0.5f, 0.5f);
    mLight.diffuseStrength = glm::vec3(0.75f, 0.75f, 0.75f);
    mLight.specularStrength = glm::vec3(1.0f, 1.0f, 1.0f);

    mHitBox.center = position;
    mHitBox.lengthOX = 2.f / 3.4f;
    mHitBox.lengthOZ = 2.f / 1.615f;
    mHitBox.lengthOY = 0.6f;
    mHitBox.id = Tank::GetNewId();
}

void Tank::SetRotPivots() {
    mTransform.rotationPivotY = glm::vec3(0.0f, 0.0f, -0.1f);

    mGunRotXPivot = glm::vec3(0.0f, -0.16f, -0.24f);

    mBigWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.156f, 0.518f));
    mBigWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.143f, -0.521f));

    mMediumWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.226f, -0.399f));
    mMediumWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.226f, -0.240f));
    mMediumWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.226f, -0.067f));
    mMediumWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.226f, 0.096f));
    mMediumWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.226f, 0.245f));
    mMediumWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.226f, 0.382f));

    mSmallWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.114f, 0.357f));
    mSmallWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.114f, 0.051f));
    mSmallWheelsRotXPivots.push_back(glm::vec3(0.0f, 0.114f, -0.273f));

    mTrackRotXPivot = glm::vec3(0.0f, 0.28247f, 0.374975f);
}

void Tank::GetTrackCurve(const std::string& path) {
    // read curve points from file
    std::ifstream file(path);

    std::string line;

    while (getline(file, line)) {
        std::stringstream lineSs(line);

        char c;
        lineSs >> c;

        glm::vec3 vertex;
        lineSs >> vertex.x >> vertex.y >> vertex.z;

        vertex.x = 0.0f;

        mTrackCurve.points.push_back({vertex, 0.0f});        
    }

    file.close();

    // get origin
    mTrackCurve.origin = mTrackCurve.points[0].position;

    unsigned int idx;

    // compute length
    for (idx = 0; idx < mTrackCurve.points.size() - 1; idx++) {
        mTrackCurve.points[idx].distToNext = glm::distance(mTrackCurve.points[idx].position,
                                                         mTrackCurve.points[idx + 1].position);
        mTrackCurve.length += mTrackCurve.points[idx].distToNext;

        float slope = (mTrackCurve.points[idx + 1].position.y - mTrackCurve.points[idx].position.y) / 
                      (mTrackCurve.points[idx + 1].position.z - mTrackCurve.points[idx].position.z);

        mTrackCurve.points[idx].slopeToNext = slope;


    }

    mTrackCurve.points[idx].distToNext = glm::distance(mTrackCurve.points[idx].position,
                                                       mTrackCurve.points[0].position);
    mTrackCurve.length += mTrackCurve.points[idx].distToNext;
    
    float slope = (mTrackCurve.points[0].position.y - mTrackCurve.points[idx].position.y) / 
                  (mTrackCurve.points[0].position.z - mTrackCurve.points[idx].position.z);

    mTrackCurve.points[idx].slopeToNext = slope;
    mTrackCurve.trackPieceLength = mTrackCurve.length / mTrackPiecesCount;
    
    // compute rotation angle

    for (idx = 1; idx < mTrackCurve.points.size(); idx++) {
        float slope1 = mTrackCurve.points[idx - 1].slopeToNext;
        float slope2 = mTrackCurve.points[idx].slopeToNext;

        mTrackCurve.points[idx].angle = mTrackCurve.points[idx - 1].angle
                                        -glm::degrees(glm::atan((slope2 - slope1) / (1  + slope1 * slope2)));
    }
}

void Tank::InitTracks() {
    GetTrackCurve("../assets/models/tank/track_curve.txt");

    for (int idx = 0; idx < mTrackPiecesCount; idx++) {
        TrackPiece trackPiece{};
        trackPiece.distFromCurveOrigin += idx * (mTrackCurve.length / mTrackPiecesCount);
        mTrackCurve.ComputeTransform(trackPiece);

        mTrackPieces.push_back(trackPiece);
    }
}

void Tank::SetModels() {
    mHull = gResourceManager->GetModel("tank_hull");
    mTurret = gResourceManager->GetModel("tank_turret");
    mGun = gResourceManager->GetModel("tank_gun");
    mTrackPiece = gResourceManager->GetModel("tank_tracks");
    mWheelsMisc = gResourceManager->GetModel("tank_wheels_misc");

    for (int i = 0; i < 2; i++) {
        mBigWheels.push_back(gResourceManager->GetModel("tank_big_wheels" + std::to_string(i)));
    }

    for (int i = 0; i < 6; i++) {
        mMediumWheels.push_back(gResourceManager->GetModel("tank_medium_wheels" + std::to_string(i)));
    }
    
    for (int i = 0; i < 3; i++) {
        mSmallWheels.push_back(gResourceManager->GetModel("tank_small_wheels" + std::to_string(i)));
    }
}

int Tank::Move(float deltaTime, std::vector<HitBox> buildingsHitBox, std::vector<HitBox> tankHitBoxes, bool closeToPlayer) {
    mTransform.position += mMovementDirection * deltaTime * mMovementSpeed;
    mHitBox.center = mTransform.position - mTransform.rotationPivotY - 0.1f * mMovementDirection;

    // hit map border
    if (mTransform.position.x <= -50.f || mTransform.position.x >= 50.f ||
        mTransform.position.z <= -50.f || mTransform.position.z >= 50.f) {
        mTransform.position -= mMovementDirection * deltaTime * mMovementSpeed;
        mHitBox.center = mTransform.position - mTransform.rotationPivotY - 0.1f * mMovementDirection;

        return 0;
    }

    for (auto& hitBox : buildingsHitBox) {
        if (mHitBox.CheckCollision(hitBox)) {
            mTransform.position -= mMovementDirection * deltaTime * mMovementSpeed;
            mHitBox.center = mTransform.position - mTransform.rotationPivotY - 0.1f * mMovementDirection;

            return 0;
        }
    }

    for (auto& hitBox : tankHitBoxes) {
        if (hitBox.id == mHitBox.id)
            continue;

        if (mHitBox.CheckCollision(hitBox)) {
            mTransform.position -= mMovementDirection * deltaTime * mMovementSpeed;
            mHitBox.center = mTransform.position - mTransform.rotationPivotY - 0.1f * mMovementDirection;

            return 0;
        }
    }

    if (!closeToPlayer)
        return 1;

    mSmallWheelsRotX += deltaTime * mSmallWheelsRotSpeed;
    mMediumWheelsRotX += deltaTime * mMediumWheelsRotSpeed;
    mBigWheelsRotX += deltaTime * mBigWheelsRotSpeed;

    for (auto& trackPiece : mTrackPieces) {
        trackPiece.Move(-deltaTime * mTracksMovementSpeed, mTrackCurve.length);
        mTrackCurve.ComputeTransform(trackPiece);
    }

    return 1;
}

int Tank::RotateYBody(float deltaTime, std::vector<HitBox> buildingsHitBox, std::vector<HitBox> tankHitBoxes) {
    mTransform.rotation.y += deltaTime * mRotationSpeed;
    mHitBox.rotationY = mTransform.rotation.y;

    for (auto& hitBox : buildingsHitBox) {
        if (mHitBox.CheckCollision(hitBox)) {
            mTransform.rotation.y -= deltaTime * mRotationSpeed;
            mHitBox.rotationY = mTransform.rotation.y;

            return 0;
        }
    }

    for (auto& hitBox : tankHitBoxes) {
        if (hitBox.center == mHitBox.center)
            continue;
            
        if (mHitBox.CheckCollision(hitBox)) {
            mTransform.rotation.y -= deltaTime * mRotationSpeed;
            mHitBox.rotationY = mTransform.rotation.y;

            return 0;
        }
    }

    UpdateMovementDirection();
    mHitBox.center = mTransform.position - mTransform.rotationPivotY - 0.1f * mMovementDirection;
    
    UpdateGunDirection();

    return 1;
}

void Tank::RotateYTurret(float angle) {
    mTurretRotY += angle;

    if (mTurretRotY > 360.f)
        mTurretRotY -= 360.f;

    if (mTurretRotY < -360.f)
        mTurretRotY += 360.f;

    UpdateGunDirection();
}

void Tank::RotateXGun(float angle) {
    mGunRotX += angle;

    mGunRotX = glm::clamp(mGunRotX,
                          -mMaxGunRotX,
                          0.0f);

    UpdateGunDirection();
}

void Tank::UpdateMovementDirection() {
    // start looking towards Z axis
    mMovementDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    mMovementDirection = glm::rotateY(mMovementDirection, glm::radians(mTransform.rotation.y));
    mMovementDirection = glm::normalize(mMovementDirection);
}

void Tank::UpdateGunDirection() {
    mGunDirection = glm::vec3(0.0f, 0.0f, 1.0f);

    mGunDirection = glm::rotateX(mGunDirection, glm::radians(mGunRotX));
    mGunDirection = glm::rotateY(mGunDirection, glm::radians(mTransform.rotation.y + mTurretRotY));
    mGunDirection = glm::normalize(mGunDirection);
}

glm::vec3 Tank::GetCameraDirection() {
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);

    cameraDirection = glm::rotateY(cameraDirection, glm::radians(mTransform.rotation.y));
    cameraDirection = glm::normalize(cameraDirection);

    return glm::normalize(cameraDirection * 3.f + glm::vec3(0.0f, -0.4f, 0.0f));
}

glm::vec3 Tank::GetCameraPosition() {
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);

    cameraDirection = glm::rotateY(cameraDirection, glm::radians(mTransform.rotation.y));
    cameraDirection = glm::normalize(cameraDirection);

    glm::vec3 position = mDistToCamera *
                         glm::normalize(-cameraDirection +
                                        glm::vec3(0.0f, 0.4f, 0.0f)) +
                         mTransform.position - mTransform.rotationPivotY;

    return position;
}

void Tank::Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition) {
    std::shared_ptr<Shader> shader = gResourceManager->GetShader("tank_shader");

    // render body
    mHull->Render(shader, viewMatrix, mTransform, cameraPosition, mLight, 1.0f, mHp, 1);
    mWheelsMisc->Render(shader, viewMatrix, mTransform, cameraPosition, mLight, 1.0f, mHp, 0);

    // render wheels
    for (unsigned int i = 0; i < mBigWheels.size(); i++) {
        Transform3D wheelTransform = mTransform;
        wheelTransform.rotationPivotX = mBigWheelsRotXPivots[i];
        wheelTransform.rotation.x = mBigWheelsRotX;

        mBigWheels[i]->Render(shader, viewMatrix, wheelTransform, cameraPosition, mLight, 1.0f, mHp, 0);
    }

    for (unsigned int i = 0; i < mMediumWheels.size(); i++) {
        Transform3D wheelTransform = mTransform;
        wheelTransform.rotationPivotX = mMediumWheelsRotXPivots[i];
        wheelTransform.rotation.x = mMediumWheelsRotX;

        mMediumWheels[i]->Render(shader, viewMatrix, wheelTransform, cameraPosition, mLight, 1.0f, mHp, 0);
    }

    for (unsigned int i = 0; i < mSmallWheels.size(); i++) {
        Transform3D wheelTransform = mTransform;
        wheelTransform.rotationPivotX = mSmallWheelsRotXPivots[i];
        wheelTransform.rotation.x = mSmallWheelsRotX;

        mSmallWheels[i]->Render(shader, viewMatrix, wheelTransform, cameraPosition, mLight, 1.0f, mHp, 0);
    }

    // render turret and gun
    Transform3D turretTransform = mTransform;
    turretTransform.rotation.y += mTurretRotY;

    mTurret->Render(shader, viewMatrix, turretTransform, cameraPosition, mLight, 1.0f, mHp, 1);

    mProjectileTransform = turretTransform;
    mProjectileTransform.rotationPivotY -= mGunRotXPivot;
    mProjectileTransform.rotationPivotY.z += 0.44f;

    mProjectileTransform.rotationPivotX = mGunRotXPivot;
    mProjectileTransform.rotationPivotX -= mGunRotXPivot;
    mProjectileTransform.rotationPivotX.z += 0.44f;

    mProjectileTransform.rotation.x = mGunRotX;

    mProjectileTransform.position -= mGunRotXPivot;
    mProjectileTransform.position.z += 0.44f;

    Transform3D gunTransform = turretTransform;
    gunTransform.rotationPivotX = mGunRotXPivot;
    gunTransform.rotation.x = mGunRotX;

    mGun->Render(shader, viewMatrix, gunTransform, cameraPosition, mLight, 1.0f, mHp, 0);


    // render track
    for (auto& trackPiece : mTrackPieces) {
        Transform3D trackPieceTransform = mTransform;

        trackPieceTransform.position.y += trackPiece.transform.position.y;
        trackPieceTransform.position += trackPiece.transform.position.z * mMovementDirection;

        trackPieceTransform.rotation.x = trackPiece.transform.rotation.x;
        trackPieceTransform.rotationPivotX = mTrackRotXPivot;

        mTrackPiece->Render(shader, viewMatrix, trackPieceTransform, cameraPosition, mLight, 1.0f, mHp, 0);
    }
}

void Tank::RenderStencil(glm::mat4 viewMatrix, glm::vec3 cameraPosition) {
    std::shared_ptr<Shader> shader = gResourceManager->GetShader("tank_shader");

    // render body
    mHull->RenderStencil(shader, viewMatrix, mTransform, cameraPosition, mLight, mHp, 1);
    mWheelsMisc->RenderStencil(shader, viewMatrix, mTransform, cameraPosition, mLight, mHp, 0);

    // render wheels
    for (unsigned int i = 0; i < mBigWheels.size(); i++) {
        Transform3D wheelTransform = mTransform;
        wheelTransform.rotationPivotX = mBigWheelsRotXPivots[i];
        wheelTransform.rotation.x = mBigWheelsRotX;

        mBigWheels[i]->RenderStencil(shader, viewMatrix, wheelTransform, cameraPosition, mLight, mHp, 0);
    }

    for (unsigned int i = 0; i < mMediumWheels.size(); i++) {
        Transform3D wheelTransform = mTransform;
        wheelTransform.rotationPivotX = mMediumWheelsRotXPivots[i];
        wheelTransform.rotation.x = mMediumWheelsRotX;

        mMediumWheels[i]->RenderStencil(shader, viewMatrix, wheelTransform, cameraPosition, mLight, mHp, 0);
    }

    for (unsigned int i = 0; i < mSmallWheels.size(); i++) {
        Transform3D wheelTransform = mTransform;
        wheelTransform.rotationPivotX = mSmallWheelsRotXPivots[i];
        wheelTransform.rotation.x = mSmallWheelsRotX;

        mSmallWheels[i]->RenderStencil(shader, viewMatrix, wheelTransform, cameraPosition, mLight, mHp, 0);
    }

    // render turret and gun
    Transform3D turretTransform = mTransform;
    turretTransform.rotation.y += mTurretRotY;

    mTurret->RenderStencil(shader, viewMatrix, turretTransform, cameraPosition, mLight, mHp, 1);

    mProjectileTransform = turretTransform;
    mProjectileTransform.rotationPivotY -= mGunRotXPivot;
    mProjectileTransform.rotationPivotY.z += 0.44f;

    mProjectileTransform.rotationPivotX = mGunRotXPivot;
    mProjectileTransform.rotationPivotX -= mGunRotXPivot;
    mProjectileTransform.rotationPivotX.z += 0.44f;

    mProjectileTransform.rotation.x = mGunRotX;

    // now projectile is at the end of the gun, with 0 rotations
    mProjectileTransform.position -= mGunRotXPivot;
    mProjectileTransform.position.z += 0.44f;

    Transform3D gunTransform = turretTransform;
    gunTransform.rotationPivotX = mGunRotXPivot;
    gunTransform.rotation.x = mGunRotX;

    mGun->RenderStencil(shader, viewMatrix, gunTransform, cameraPosition, mLight, mHp, 0);

    // render track
    for (auto& trackPiece : mTrackPieces) {
        Transform3D trackPieceTransform = mTransform;

        trackPieceTransform.position.y += trackPiece.transform.position.y;
        trackPieceTransform.position += trackPiece.transform.position.z * mMovementDirection;

        trackPieceTransform.rotation.x = trackPiece.transform.rotation.x;
        trackPieceTransform.rotationPivotX = mTrackRotXPivot;

        mTrackPiece->RenderStencil(shader, viewMatrix, trackPieceTransform, cameraPosition, mLight, mHp, 0);
    }
}

void Tank::Fire(std::shared_ptr<ProjectileManager> projectileManager) {
    projectileManager->AddProjectile(mGunDirection, mProjectileTransform, mHitBox.id);
}