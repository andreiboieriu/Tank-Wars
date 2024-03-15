#include <core/game.hpp>

#include <utils/random_generator.hpp>
#include <utils/resource_manager.hpp>

#include <glad/glad.h>

#include <core/main_scene.hpp>
#include <core/loading_scene.hpp>

// std
#include <chrono>
#include <memory>
#include <iostream>
#include <algorithm>
#include <thread>

std::unique_ptr<RandomGenerator> gRandomGenerator;
std::unique_ptr<ResourceManager> gResourceManager;

void Game::Init() {
    mWindow = std::make_unique<Window>();
    mWindow->Init("World of Tanks", 1600, 900);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    // configure OpenGL
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // gamma correction
    glEnable(GL_FRAMEBUFFER_SRGB);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);    

    // create resource manager and random generator
    gResourceManager = std::make_unique<ResourceManager>();
    gRandomGenerator = std::make_unique<RandomGenerator>();

    mLoadingScene = std::make_unique<LoadingScene>();
    mMainScene = std::make_unique<MainScene>();
}

void Game::LoadMainScene() {
    mMainScene->Load();
    mFinishedLoading = true;
}

void Game::RunLoadingAnimation() {
    mLoadingScene->Run(mFinishedLoading);
}

void Game::Run() {
    mLoadingScene->SetWindow(std::move(mWindow));
    mLoadingScene->Load();

    auto f = [&](Game *game) {
        game->LoadMainScene();
    };

    std::thread t(f, this);

    RunLoadingAnimation();

    t.join();
    std::cout << "I/O operations finished" << std::endl;

    mLoadingScene->RunAndLoad();

    mMainScene->SetWindow(std::move(mLoadingScene->GetWindow()));
    mFinishedLoading = false;
    mMainScene->Run(mFinishedLoading);

    mWindow = std::move(mMainScene->GetWindow());
}

