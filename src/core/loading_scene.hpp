#pragma once

#include <core/scene.hpp>
#include <core/window.hpp>

#include <memory>

class LoadingScene : public Scene {

public:
    virtual void Load() override;
    virtual void Init() override {
        
    }

    void Run(bool& stop) {
        Scene::Run(stop);
    }

    void RunAndLoad();

    virtual void OnMouseBtnPress(int button, int action, int mods) override;
    virtual void OnMouseMove(double xPos, double yPos) override;
    virtual void OnKeyPressed(int key, int scancode, int action, int mode) override;

private:
    virtual void FrameStart() override;
    virtual void Update(float deltaTime) override;
    virtual void FrameEnd() override;

    enum class State {
        LOADING0,
        LOADING1,
        LOADING2,
        LOADING3
    };

    State mState = State::LOADING0;

    const float mStateDuration = 1.f;
};