#pragma once

#include <core/window.hpp>
#include <core/scene.hpp>
#include <entities/player.hpp>
#include <entities/ground.hpp>
#include <entities/enemy.hpp>
#include <entities/building_manager.hpp>
#include <entities/projectile_manager.hpp>
#include <entities/enemy_manager.hpp>
#include <utils/hitbox.hpp>


#include <memory>

class MainScene : public Scene {

public:
    virtual void Load() override;
    virtual void Init() override;

    void Run(bool& stop) {
        Scene::Run(stop);
    }

    virtual void OnMouseBtnPress(int button, int action, int mods) override;
    virtual void OnMouseMove(double xPos, double yPos) override;
    virtual void OnKeyPressed(int key, int scancode, int action, int mode) override;

    struct MyVertex {
        glm::vec3 coords;

        MyVertex(float x, float y, float z) {
            coords.x = x;
            coords.y = y;
            coords.z = z;
        };
    };

private:
    virtual void FrameStart() override;
    virtual void Update(float deltaTime) override;
    virtual void FrameEnd() override;

    void GameUpdate(float deltaTime);
    void MenuUpdate(float deltaTime);
    void TransitionUpdate(float deltaTime);
    void EndMenuUpdate(float deltaTime);

    std::shared_ptr<Player> mPlayer;
    std::shared_ptr<ProjectileManager> mProjectileManager;
    std::unique_ptr<Ground> mGround;
    std::unique_ptr<BuildingManager> mBuildingManager;
    std::unique_ptr<EnemyManager> mEnemyManager;

    glm::vec2 mMouseCoords;

    enum class State {
        MENU,
        TRANSITION,
        GAME,
        END_MENU
    };

    State mState = State::MENU;

    const float mGameDuration = 180.f;
    float mGameTimeElapsed = 0.f;

    int mScore = 0;

    float mTransitionTimeElapsed = 0.f;
    const float mTransitionDuration = 3.f;
};