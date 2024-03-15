#pragma once

#include <graphics/model.hpp>
#include <core/camera.hpp>
#include <core/scene.hpp>
#include <core/loading_scene.hpp>
#include <core/window.hpp>

#include <entities/player.hpp>

// std
#include <memory>

class Game {

public:
    ~Game() {
        mWindow->Exit();
    };

    void Init();
    void Run();

private:
    void LoadMainScene();
    void RunLoadingAnimation();

    std::unique_ptr<LoadingScene> mLoadingScene;
    std::unique_ptr<Scene> mMainScene;

    float deltaTime = 0.f;
    std::unique_ptr<Window> mWindow;
    glm::vec2 mMousePos;

    std::unique_ptr<Camera> mCamera;
    std::shared_ptr<Player> mPlayer;

    int mCursorState = GLFW_CURSOR_NORMAL;

    Transform3D mTransform{};
    bool mFinishedLoading = false;
};