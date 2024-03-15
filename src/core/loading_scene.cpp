#include <core/loading_scene.hpp>
#include <utils/resource_manager.hpp>
#include <utils/constants.hpp>

#include <chrono>

void LoadingScene::Load() {
    gResourceManager->LoadMesh2D("square", Mesh2D::CreateSquare());

    gResourceManager->LoadShader("text_shader", "../shaders/text_vertex.glsl", "../shaders/text_fragment.glsl");
    gResourceManager->LoadShader("model_shader", "../shaders/model_vertex.glsl", "../shaders/model_fragment.glsl");
    gResourceManager->LoadShader("ground_shader", "../shaders/ground_vertex.glsl", "../shaders/ground_fragment.glsl");
    gResourceManager->LoadShader("simple_shader", "../shaders/simple_vertex.glsl", "../shaders/simple_fragment.glsl");
    gResourceManager->LoadShader("building_shader", "../shaders/building_vertex.glsl", "../shaders/building_fragment.glsl");
    gResourceManager->LoadShader("particle_shader", "../shaders/particle_vertex.glsl", "../shaders/particle_fragment.glsl");
    gResourceManager->LoadShader("stencil_shader", "../shaders/stencil_vertex.glsl", "../shaders/stencil_fragment.glsl");
    gResourceManager->LoadShader("tank_shader", "../shaders/tank_vertex.glsl", "../shaders/tank_fragment.glsl");
    gResourceManager->LoadShader("sprite_shader", "../shaders/sprite_vertex.glsl", "../shaders/sprite_fragment.glsl");

    gResourceManager->LoadShader("trajectory_shader", "../shaders/trajectory_vertex.glsl", "../shaders/trajectory_fragment.glsl");


    gResourceManager->LoadTexture("button", "../assets/textures/button.png");

    gResourceManager->LoadSprite("button", gResourceManager->GetMesh2D("square"), gResourceManager->GetTexture("button"));
    gResourceManager->LoadFont("../assets/fonts/arial.ttf");
}

void LoadingScene::RunAndLoad() {
    float deltaTime = 0.f;

    while (!mWindow->ShouldClose()) {
        using namespace std::chrono;

        auto startTime = high_resolution_clock::now();

        mWindow->ProcessEvents();

        if (!gResourceManager->LoadBufferedModels())
            break;
            
        FrameStart();
        Update(deltaTime);
        FrameEnd();

        mWindow->Update();

        auto stopTime = high_resolution_clock::now();

        deltaTime = duration<float, seconds::period>(stopTime - startTime).count();
    }
}

void LoadingScene::FrameStart() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void LoadingScene::Update(float deltaTime) {
    mElapsedTime += deltaTime;

    float length = 0.f;
    Transform2D transform{};
    transform.position = glm::vec3(LOGIC_WIDTH / 2, LOGIC_HEIGHT / 2, 0.f);
    transform.scale = glm::vec2(1.f);

    switch (mState) {
    case State::LOADING0:
        gResourceManager->RenderText("Loading", transform, glm::vec3(0.f), 1.0f, length);

        if (mElapsedTime > mStateDuration) {
            mElapsedTime -= mStateDuration;
            mState = State::LOADING1;
        }

        break;

    case State::LOADING1:
        gResourceManager->RenderText("Loading.", transform, glm::vec3(0.f), 1.0f, length);

        if (mElapsedTime > mStateDuration) {
            mElapsedTime -= mStateDuration;
            mState = State::LOADING2;
        }

        break;

    case State::LOADING2:
        gResourceManager->RenderText("Loading..", transform, glm::vec3(0.f), 1.0f, length);

        if (mElapsedTime > mStateDuration) {
            mElapsedTime -= mStateDuration;
            mState = State::LOADING3;
        }

        break;

    case State::LOADING3:
        gResourceManager->RenderText("Loading...", transform, glm::vec3(0.f), 1.0f, length);

        if (mElapsedTime > mStateDuration) {
            mElapsedTime -= mStateDuration;
            mState = State::LOADING0;
        }

        break;

    };
}

void LoadingScene::FrameEnd() {

}

void LoadingScene::OnMouseBtnPress(int button, int action, int mods) {

}

void LoadingScene::OnMouseMove(double xPos, double yPos) {
    
}

void LoadingScene::OnKeyPressed(int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (mCursorState == GLFW_CURSOR_NORMAL) {
            mCursorState = GLFW_CURSOR_DISABLED;
        } else {
            mCursorState = GLFW_CURSOR_NORMAL;
        }

        // disable mouse cursor
        glfwSetInputMode(mWindow->GetHandle(), GLFW_CURSOR, mCursorState);
    }
}