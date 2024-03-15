#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


// std
#include <string>
#include <memory>

class Window {

public:
    void Init(std::string const& title, int width, int height);

    void Update();

    void ProcessEvents();

    void Exit();

    bool ShouldClose() {
        return glfwWindowShouldClose(mWindow);
    }

    GLFWwindow *GetHandle() {
        return mWindow;
    }

    glm::ivec2 GetResolution() {
        glm::ivec2 res;
        glfwGetFramebufferSize(mWindow, &res.x, &res.y);

        return res;
    }

private:
    GLFWwindow *mWindow;
};