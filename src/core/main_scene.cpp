#include <core/main_scene.hpp>
#include <utils/resource_manager.hpp>

#include <chrono>

void MainScene::Load() {
    gResourceManager->LoadModel("cube", "../assets/models/cube/scene.gltf", true);
    gResourceManager->LoadModel("ground_tile", "../assets/models/forest_ground/ground_tile.gltf", true);

    gResourceManager->LoadModel("tank_gun", "../assets/models/tank/gun.gltf", true);
    gResourceManager->LoadModel("tank_turret", "../assets/models/tank/turret.gltf", true);
    gResourceManager->LoadModel("tank_hull", "../assets/models/tank/hull.gltf", true);

    gResourceManager->LoadModel("tank_big_wheels0", "../assets/models/tank/big_wheels_0.gltf", true);
    gResourceManager->LoadModel("tank_big_wheels1", "../assets/models/tank/big_wheels_1.gltf", true);

    gResourceManager->LoadModel("tank_tracks", "../assets/models/tank/single_track_2.gltf", true);

    gResourceManager->LoadModel("tank_small_wheels0", "../assets/models/tank/small_wheels_0.gltf", true);
    gResourceManager->LoadModel("tank_small_wheels1", "../assets/models/tank/small_wheels_1.gltf", true);
    gResourceManager->LoadModel("tank_small_wheels2", "../assets/models/tank/small_wheels_2.gltf", true);

    gResourceManager->LoadModel("tank_medium_wheels0", "../assets/models/tank/medium_wheels_0.gltf", true);
    gResourceManager->LoadModel("tank_medium_wheels1", "../assets/models/tank/medium_wheels_1.gltf", true);
    gResourceManager->LoadModel("tank_medium_wheels2", "../assets/models/tank/medium_wheels_2.gltf", true);
    gResourceManager->LoadModel("tank_medium_wheels3", "../assets/models/tank/medium_wheels_3.gltf", true);
    gResourceManager->LoadModel("tank_medium_wheels4", "../assets/models/tank/medium_wheels_4.gltf", true);
    gResourceManager->LoadModel("tank_medium_wheels5", "../assets/models/tank/medium_wheels_5.gltf", true);

    gResourceManager->LoadModel("tank_wheels_misc", "../assets/models/tank/wheels_misc.gltf", true);

    gResourceManager->LoadModel("building", "../assets/models/old_house_ruin/decimated_3.gltf", true);
    gResourceManager->LoadModel("tank_projectile", "../assets/models/tank_ammo_heat/transformed_2.gltf", true);
}

void MainScene::Init() {
    Transform3D transform{};
    transform.scale = glm::vec3(1.f);
    transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    mProjectileManager = std::make_shared<ProjectileManager>();

    mPlayer = std::make_shared<Player>(transform, mProjectileManager);
    mGround = std::make_unique<Ground>();
    mBuildingManager = std::make_unique<BuildingManager>(25);
    mEnemyManager = std::make_unique<EnemyManager>(mProjectileManager, mBuildingManager->GetHitBoxes(), mPlayer);
}

void MainScene::FrameStart() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void MainScene::Update(float deltaTime) {
    switch (mState)
    {
    case State::MENU:
        MenuUpdate(deltaTime);
        break;

    case State::TRANSITION:
        TransitionUpdate(deltaTime);
        break;
    
    case State::GAME:
        GameUpdate(deltaTime);
        break;
    
    case State::END_MENU:
        EndMenuUpdate(deltaTime);
        break;
    }
}

void MainScene::EndMenuUpdate(float deltaTime) {
    glm::mat4 viewMatrix = mPlayer->GetViewMatrix();
    glm::vec3 cameraPosition = mPlayer->GetCameraPosition();

    mGround->Render(viewMatrix, cameraPosition);
    mBuildingManager->Render(viewMatrix, cameraPosition);
    mEnemyManager->Render(viewMatrix, cameraPosition);
    mProjectileManager->Render(viewMatrix, cameraPosition);

    std::vector<HitBox> tankHitBoxes;

    mPlayer->Render(tankHitBoxes, mBuildingManager->GetHitBoxes());

    // render buttons and text
    Transform2D textTransform{};
    textTransform.position = glm::vec3(LOGIC_WIDTH * 0.5f, LOGIC_HEIGHT * 0.7, 0.f);
    textTransform.scale = glm::vec3(1.f);

    float length = 0.f;

    if (mPlayer->IsDead())
        gResourceManager->RenderText("You Died", textTransform, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, length);
    else
        gResourceManager->RenderText("Game Ended", textTransform, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, length);

    textTransform.position = glm::vec3(LOGIC_WIDTH * 0.5f, LOGIC_HEIGHT * 0.65, 0.f);

    gResourceManager->RenderText("Final Score: " + std::to_string(mScore), textTransform, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, length);

}

void MainScene::MenuUpdate(float deltaTime) {
    glm::vec3 cameraDirection = glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f) * 3.f + glm::vec3(0.0f, -0.2f, 0.0f));

    mPlayer->SetCameraDirection(cameraDirection);

    glm::vec3 position = 2.f * glm::normalize(-glm::vec3(-1.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.2f, 0.0f)) + glm::vec3(0.0f, 0.0f, .7f);
    
    mPlayer->SetCameraPosition(position);

    Transform2D textTransform{};
    textTransform.position = glm::vec3(LOGIC_WIDTH * 0.2f, LOGIC_HEIGHT * 3.f / 4, 0.f);
    textTransform.scale = glm::vec3(400.f, 150.f, 1.f);

    auto button = gResourceManager->GetSprite("button");
    auto buttonShader = gResourceManager->GetShader("sprite_shader");
    button->Render(buttonShader, textTransform, glm::vec3(1.f), 1.0f);

    textTransform.position = glm::vec3(LOGIC_WIDTH * 0.2f, LOGIC_HEIGHT * 3.f / 4, 2.f);
    textTransform.scale = glm::vec3(2.f);

    float length = 0.f;
    gResourceManager->RenderText("Play", textTransform, glm::vec3(0.f), 1.0f, length);


    glm::mat4 viewMatrix = mPlayer->GetViewMatrix();
    glm::vec3 cameraPosition = mPlayer->GetCameraPosition();

    mGround->Render(viewMatrix, cameraPosition);
    mBuildingManager->Render(viewMatrix, cameraPosition);

    std::vector<HitBox> hitBoxes;

    mPlayer->Render(hitBoxes, mBuildingManager->GetHitBoxes());
}

void MainScene::GameUpdate(float deltaTime) {
    std::vector<HitBox> tankHitBoxes = mEnemyManager->GetHitBoxes();

    mGameTimeElapsed += deltaTime;

    mPlayer->Update(deltaTime, mBuildingManager->GetHitBoxes(), tankHitBoxes, mEnemyManager->GetEnemies());

    glm::mat4 viewMatrix = mPlayer->GetViewMatrix();
    glm::vec3 cameraPosition = mPlayer->GetCameraPosition();

    mGround->Render(viewMatrix, cameraPosition);
    mBuildingManager->Render(viewMatrix, cameraPosition);

    tankHitBoxes.push_back(mPlayer->GetHitBox());

    mProjectileManager->Update(deltaTime, mBuildingManager->GetHitBoxes(), mEnemyManager->GetEnemies(), mPlayer, mScore);

    mEnemyManager->Update(deltaTime, mBuildingManager->GetHitBoxes(), mPlayer);
    mEnemyManager->Render(viewMatrix, cameraPosition);

    mProjectileManager->Render(viewMatrix, cameraPosition);

    tankHitBoxes.pop_back();

    mPlayer->Render(tankHitBoxes, mBuildingManager->GetHitBoxes());
    
    // render text
    Transform2D textTransform{};
    textTransform.position = glm::vec3(LOGIC_WIDTH * 0.95f, LOGIC_HEIGHT * 0.98, 0.f);
    textTransform.scale = glm::vec3(.5f);

    float length = 0.f;
    gResourceManager->RenderText("Time left: " + std::to_string(static_cast<int>(mGameDuration - mGameTimeElapsed)),
                                 textTransform, glm::vec3(0.f), 1.0f, length);

    textTransform.position = glm::vec3(LOGIC_WIDTH * 0.95f, LOGIC_HEIGHT * 0.95, 0.f);

    gResourceManager->RenderText("Score: " + std::to_string(mScore),
                                 textTransform, glm::vec3(0.f), 1.0f, length);

    if (mPlayer->IsDead() || mGameTimeElapsed >= mGameDuration) {
        mState = State::END_MENU;
        mPlayer->DisableTrajectory();

        mCursorState = GLFW_CURSOR_NORMAL;
        glfwSetInputMode(mWindow->GetHandle(), GLFW_CURSOR, mCursorState);
    }
}

void MainScene::TransitionUpdate(float deltaTime) {
    glm::vec3 initialCameraDirection = glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f) * 3.f + glm::vec3(0.0f, -0.2f, 0.0f));
    glm::vec3 initialCameraPosition = 2.f * glm::normalize(-glm::vec3(-1.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.2f, 0.0f)) + glm::vec3(0.0f, 0.0f, .7f);

    glm::vec3 targetCameraDirection = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f) * 3.f + glm::vec3(0.0f, -0.4f, 0.0f));
    glm::vec3 targetCameraPosition = 2.f * glm::normalize(-glm::vec3(0.0f, 0.0f, 1.0f) + glm::vec3(0.0f, 0.4f, 0.0f)) - glm::vec3(0.0f, 0.0f, -0.1f);

    // linear interpolation
    mTransitionTimeElapsed += deltaTime;

    float t = mTransitionTimeElapsed / mTransitionDuration;

    if (t >= 1.0f) {
        t = 1.0f;
    }

    glm::vec3 cameraDirection = (1.0f - t) * initialCameraDirection + t * targetCameraDirection;
    glm::vec3 cameraPosition = (1.0f - t) * initialCameraPosition + t * targetCameraPosition;

    mPlayer->SetCameraDirection(cameraDirection);
    mPlayer->SetCameraPosition(cameraPosition);

    // render scene
    glm::mat4 viewMatrix = mPlayer->GetViewMatrix();

    mGround->Render(viewMatrix, cameraPosition);
    mBuildingManager->Render(viewMatrix, cameraPosition);

    std::vector<HitBox> tankHitBoxes = mEnemyManager->GetHitBoxes();

    tankHitBoxes.push_back(mPlayer->GetHitBox());

    mProjectileManager->Update(deltaTime, mBuildingManager->GetHitBoxes(), mEnemyManager->GetEnemies(), mPlayer, mScore);

    mEnemyManager->Update(deltaTime, mBuildingManager->GetHitBoxes(), mPlayer);
    mEnemyManager->Render(viewMatrix, cameraPosition);

    mProjectileManager->Render(viewMatrix, cameraPosition);

    tankHitBoxes.pop_back();

    mPlayer->Render(tankHitBoxes, mBuildingManager->GetHitBoxes());

    if (mTransitionTimeElapsed >= mTransitionDuration) {
        mState = State::GAME;

        mCursorState = GLFW_CURSOR_DISABLED;
        glfwSetInputMode(mWindow->GetHandle(), GLFW_CURSOR, mCursorState);
    }
}

void MainScene::FrameEnd() {

}

void MainScene::OnMouseBtnPress(int button, int action, int mods) {
    if (mState == State::GAME)
        mPlayer->OnMouseBtnPress(button, action, mods);

    if (action == GLFW_PRESS && mState == State::MENU && button == GLFW_MOUSE_BUTTON_LEFT) {
        glm::vec2 buttonCenter = glm::vec2(LOGIC_WIDTH * 0.2f, LOGIC_HEIGHT * 3.f / 4.f);

        if (mMouseCoords.x <= buttonCenter.x + 200.f && mMouseCoords.x >= buttonCenter.x - 200.f &&
            mMouseCoords.y <= buttonCenter.y + 75.f && mMouseCoords.y >= buttonCenter.y - 75.f) {
            mState = State::TRANSITION;
        }
    }
}

void MainScene::OnMouseMove(double xPos, double yPos) {
    glm::vec2 mouseCoords = GetLogicCoords(glm::vec2(xPos, yPos));
    mMouseCoords = mouseCoords;

    if (mState == State::GAME)
        mPlayer->OnMouseMove(mouseCoords);
}

void MainScene::OnKeyPressed(int key, int scancode, int action, int mode) {
    if (mState == State::GAME)
        mPlayer->OnKeyPressed(key, scancode, action, mode);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        mStop = true;
    }
}