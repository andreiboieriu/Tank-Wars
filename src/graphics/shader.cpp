#include <graphics/shader.hpp>
#include <utils/shader_utils.hpp>

// std
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // store source code from files here
    std::string vertexContents = readFileContents(vertexPath);
    std::string fragmentContents = readFileContents(fragmentPath);

    int success;
    char infoLog[512];

    // compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSource = vertexContents.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader compilation failed: " << infoLog << std::endl;
    }

    // compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* fragmentShaderSource = fragmentContents.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader compilation failed: " << infoLog << std::endl;
    }

    // create shader program and attach shaders
    mId = glCreateProgram();

    glAttachShader(mId, vertexShader);
    glAttachShader(mId, fragmentShader);

    // link program
    glLinkProgram(mId);

    glGetProgramiv(mId, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(mId, 512, NULL, infoLog);
        std::cerr << "Failed to link shader program: " << infoLog << std::endl;
    }

    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() const {
    glUseProgram(mId);
}
