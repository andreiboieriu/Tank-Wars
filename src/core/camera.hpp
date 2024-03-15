#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
    void OnMouseMove(glm::vec2 mouseCoords);

    glm::mat4 GetViewMatrix() {
        return mViewMatrix;
    }

    void SetPosition(glm::vec3 position) {
        mPosition = position;
        UpdateViewMatrix();
    }

    void SetDirection(glm::vec3 direction) {
        mDirection = direction;
        UpdateViewMatrix();
    }

    void FreeView(float deltaTime, bool firstFrame);

    void UpdateViewMatrix() {
        mViewMatrix = glm::lookAt(mPosition, mPosition + mDirection, mUp);
    }

    glm::vec3 GetPosition() {
        return mPosition;
    }

private:

    glm::vec3 mPosition;
    glm::vec3 mDirection;

    glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 mRight;

    glm::mat4 mViewMatrix;

    float mFreeViewX = 0.0f;
    float mFreeViewY = 0.0f;

    const float mFreeViewMaxX = 45.f;
    const float mFreeViewMinX = -45.f;

    const float mFreeViewMaxY = 90.f;
    const float mFreeViewMinY = -90.f;

    glm::vec2 mLastMouseCoords;
    glm::vec2 mCurrentMouseCoords;
    bool mFirstTimeMovingMouse = true;

    const float mSpeed = 5.f;
    const float mSensitivity = 20.f;

    bool mMouseMoved = false;
};