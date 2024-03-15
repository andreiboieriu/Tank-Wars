#pragma once

#include <utils/transform.hpp>
#include <utils/hitbox.hpp>

#include <entities/projectile_manager.hpp>

#include <graphics/light.hpp>

#include <graphics/model.hpp>
#include <vector>

class ProjectileManager;

class Tank {

public:
    Tank(glm::vec3 position);

    int Move(float deltaTime, std::vector<HitBox> buildingsHitBox, std::vector<HitBox> tankHitBoxes, bool closeToPlayer);
    int RotateYBody(float deltaTime, std::vector<HitBox> buildingsHitBox, std::vector<HitBox> tankHitBoxes);
    void RotateYTurret(float angle);
    void RotateXGun(float angle);

    void Fire(std::shared_ptr<ProjectileManager> projectileManager);

    void Render(glm::mat4 viewMatrix, glm::vec3 cameraPosition);
    void RenderStencil(glm::mat4 viewMatrix, glm::vec3 cameraPosition);

    static int GetNewId() {
        static int id = 0;
        return id++;
    }

    glm::vec3 GetCameraDirection();
    glm::vec3 GetCameraPosition();

    HitBox GetHitBox() {
        return mHitBox;
    }

    glm::vec3 GetGunDirection() {
        return mGunDirection;
    }

    Transform3D GetProjectileTransform() {
        return mProjectileTransform;
    }

    void DecrementHp() {
        mHp--;

        if (mHp < 0)
            mHp = 0;
    }

    int GetHp() {
        return mHp;
    }

    float GetGunRotX() {
        return mGunRotX;
    }

    float GetTurretRotY() {
        return mTurretRotY;
    }

private:
    void SetModels();
    void SetRotPivots();
    void InitTracks();
    void UpdateMovementDirection();
    void UpdateGunDirection();

    int mHp = 3;

    Light mLight{};

    Transform3D mTransform{};
    Transform3D mProjectileTransform{};

    glm::vec3 mMovementDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 mGunDirection = glm::vec3(0.0f, 0.0f, 1.0f);

    HitBox mHitBox{};

    float mDistToCamera = 2.f;

    float mSmallWheelsRotX = 0.f;
    float mMediumWheelsRotX = 0.f;
    float mBigWheelsRotX = 0.f;

    float mGunRotX = 0.f;
    float mTurretRotY = 0.f;

    std::shared_ptr<Model> mHull;
    std::shared_ptr<Model> mTurret;
    std::shared_ptr<Model> mGun;
    std::shared_ptr<Model> mTrackPiece;
    std::shared_ptr<Model> mWheelsMisc;

    std::vector<std::shared_ptr<Model>> mBigWheels;
    std::vector<std::shared_ptr<Model>> mMediumWheels;
    std::vector<std::shared_ptr<Model>> mSmallWheels;

    std::vector<glm::vec3> mBigWheelsRotXPivots;
    std::vector<glm::vec3> mMediumWheelsRotXPivots;
    std::vector<glm::vec3> mSmallWheelsRotXPivots;

    const float mBigWheelsRotSpeed = 70.f;
    const float mMediumWheelsRotSpeed = 70.f;
    const float mSmallWheelsRotSpeed= 70.f;
    const float mTracksMovementSpeed = .1f;

    const float mMaxGunRotX = 18.f;
    glm::vec3 mGunRotXPivot;
    glm::vec3 mTrackRotXPivot;

    float mMovementSpeed = 1.f;
    float mRotationSpeed = 50.f;

    void GetTrackCurve(const std::string& path);

    struct TrackPiece {
        Transform3D transform;
        float distFromCurveOrigin;

        void Move(float distance, float curveLength) {
            distFromCurveOrigin += distance;

            if (distFromCurveOrigin >= curveLength) {
                distFromCurveOrigin -= curveLength;
            }

            if (distFromCurveOrigin < 0) {
                distFromCurveOrigin += curveLength;
            }
        }
    };

    struct CurvePoint {
        glm::vec3 position;
        float distToNext;
        float slopeToNext;
        float angle;
    };

    struct TrackCurve {
        std::vector<CurvePoint> points;
        glm::vec3 origin;
        float length;
        float trackPieceLength;

        void ComputeTransform(TrackPiece& trackPiece) {
            float distFromCurveOrigin = trackPiece.distFromCurveOrigin;

            unsigned int p;
            for (p = 0; p < points.size(); p++) {
                if (distFromCurveOrigin >= points[p].distToNext) {
                    distFromCurveOrigin -= points[p].distToNext;
                } else {
                    break;
                }
            }

            // now obj is somewhere between p and p + 1
            p = p % points.size();
            unsigned int next = (p + 1) % points.size();
            
            float t = distFromCurveOrigin / points[p].distToNext;

            // linear interpolation between p and next
            glm::vec3 position = points[p].position * (1.0f - t) + points[next].position * t;
            trackPiece.transform.position = position - origin;

            float angle;

            if (next == 0) {
                angle = points[p].angle * (1.0f - t) + (-360.f) * t;
            } else if (next == 1) {
                angle = points[p].angle * (1.0f - t) + 0.f * t;
            } else {
                angle = points[p].angle * (1.0f - t) + points[next].angle * t;
            }

            trackPiece.transform.rotation.x = angle;
        }
    };

    std::vector<TrackPiece> mTrackPieces;
    const int mTrackPiecesCount = 85;

    TrackCurve mTrackCurve{};
};