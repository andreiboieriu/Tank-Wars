#include <core/camera.hpp>

#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>

void Camera::FreeView(float deltaTime, bool firstFrame) {
    if (firstFrame) {
        // reset free view angles
        mFreeViewX = 0.f;
        mFreeViewY = 0.f;
    }

    // update angles if mouse moved
    if (mMouseMoved) {
        float xOffset = mCurrentMouseCoords.x - mLastMouseCoords.x;
        float yOffset = mCurrentMouseCoords.y - mLastMouseCoords.y;

        xOffset *= -mSensitivity * deltaTime;
        yOffset *= -mSensitivity * deltaTime;

        mFreeViewX += xOffset;
        mFreeViewY -= yOffset;

        // clamp values
        mFreeViewX = glm::clamp(mFreeViewX, mFreeViewMinX, mFreeViewMaxX);
        mFreeViewY = glm::clamp(mFreeViewY, mFreeViewMinY, mFreeViewMaxY);
    }

    glm::vec3 right = glm::normalize(glm::cross(mDirection, glm::vec3(0.0f, 1.0f, 0.0f)));

    // rotate direction
    mDirection = glm::rotate(mDirection, glm::radians(mFreeViewY), right);
    mDirection = glm::rotateY(mDirection, glm::radians(mFreeViewX));

    mDirection = glm::normalize(mDirection);

    mMouseMoved = false;
}

void Camera::OnMouseMove(glm::vec2 mouseCoords) {
    mLastMouseCoords = mCurrentMouseCoords;
    mCurrentMouseCoords = mouseCoords;

    if (mFirstTimeMovingMouse) {
        mFirstTimeMovingMouse = false;
        return;
    }

    mMouseMoved = true;
}
