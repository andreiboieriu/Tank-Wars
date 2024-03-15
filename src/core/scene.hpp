#pragma once

#include <core/window.hpp>
#include <utils/constants.hpp>
#include <utils/transform.hpp>
#include <utils/resource_manager.hpp>

#include <memory>
#include <chrono>
#include <iostream>

class Scene {

public:
    virtual ~Scene() {}

    virtual void Load() = 0;
    virtual void Init() = 0;

    virtual void Run(bool& stop) {
        Init();

        float deltaTime = 0.f;
        float frameTime = 0.f;
        int frameCounter = 0;
        int displayedFps = 0;

        Transform2D frameCounterTransform{};
        frameCounterTransform.scale = glm::vec2(.5f);
        frameCounterTransform.position = glm::vec3(LOGIC_WIDTH * 0.02f, LOGIC_HEIGHT * 0.98f, 0.f);
        float length = 0.f;
    
        while (!mWindow->ShouldClose() && !stop && !mStop) {
            if (frameTime >= 1.f) {
                frameTime -= 1.f;
                displayedFps = frameCounter;
                frameCounter = 0;
            }

            using namespace std::chrono;

            auto startTime = high_resolution_clock::now();

            mWindow->ProcessEvents();

            FrameStart();
            Update(deltaTime);
            FrameEnd();

            gResourceManager->RenderText(std::to_string(displayedFps), frameCounterTransform, glm::vec3(0.f), 1.f, length);


            mWindow->Update();

            auto stopTime = high_resolution_clock::now();

            deltaTime = duration<float, seconds::period>(stopTime - startTime).count();

            mElapsedTime += deltaTime;
            frameTime += deltaTime;
            frameCounter++;
        }
    }

    // made public to be set as callbacks
    virtual void OnMouseBtnPress(int button, int action, int mods) = 0;
    virtual void OnMouseMove(double xPos, double yPos) = 0;
    virtual void OnKeyPressed(int key, int scancode, int action, int mode) = 0;

    void SetWindow(std::unique_ptr<Window> window) {
        mWindow = std::move(window);
        glfwSetWindowUserPointer(mWindow->GetHandle(), this);
    }

    std::unique_ptr<Window> GetWindow() {
        return std::move(mWindow);
    }

protected:
    virtual void FrameStart() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void FrameEnd() = 0;

    glm::vec2 GetLogicCoords(glm::vec2 coords) {
        glm::ivec2 resolution = mWindow->GetResolution();

        return glm::vec2(coords.x * LOGIC_WIDTH / (1.f * resolution.x),
                        LOGIC_HEIGHT - coords.y * LOGIC_HEIGHT / (1.f * resolution.y));
    }

    std::unique_ptr<Window> mWindow;
    glm::vec2 mMousePos;

    // std::unique_ptr<Camera> mCamera;
    // std::shared_ptr<Player> mPlayer;

    // Model *model;
    float mElapsedTime = 0.f;

    bool mStop = false;

    int mCursorState = GLFW_CURSOR_NORMAL;
};