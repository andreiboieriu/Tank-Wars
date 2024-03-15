#include <core/window.hpp>
#include <core/scene.hpp>

#include <glad/glad.h>

// std
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    static_cast<Scene *>(glfwGetWindowUserPointer(window))->OnKeyPressed(key, scancode, action, mode);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    static_cast<Scene *>(glfwGetWindowUserPointer(window))->OnMouseBtnPress(button, action, mods);
}

static void cursor_position_callback(GLFWwindow *window, double xPos, double yPos) {
    static_cast<Scene *>(glfwGetWindowUserPointer(window))->OnMouseMove(xPos, yPos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // reset viewport
    glViewport(0, 0, width, height);
}

void Window::Init(std::string const& title, int width, int height) {
    // initialize glfw library
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

     // set window hints
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
    glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);


    glfwWindowHint(GLFW_SAMPLES, 16);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    // create window
    mWindow = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, NULL);

    // mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);


    if (!mWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
    }

    // create OpenGL context
    glfwMakeContextCurrent(mWindow);

    // glfwSwapInterval(0);

    glfwSetKeyCallback(mWindow, key_callback);
    glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
    glfwSetCursorPosCallback(mWindow, cursor_position_callback);
    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
}

void Window::Update() {
    glfwSwapBuffers(mWindow);
}

void Window::ProcessEvents() {
    glfwPollEvents();
}

void Window::Exit() {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}