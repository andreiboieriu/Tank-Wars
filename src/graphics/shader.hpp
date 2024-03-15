#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

// std
#include <string>
#include <iostream>

class Shader {

public:
    Shader (const std::string& vertexPath, const std::string& fragmentPath);

    void Use() const;

    template<typename T>
    void SetUniform(const std::string& name, const T& value) const {
        int loc = glGetUniformLocation(mId, name.c_str());

        if (loc == -1)
            return;

        if constexpr (std::is_same_v<T, float>) {
            glUniform1f(loc, value);
        } else if constexpr (std::is_same_v<T, int>) {
            glUniform1i(loc, value);
        } else if constexpr (std::is_same_v<T, bool>) {
            glUniform1i(loc, static_cast<int>(value));
        } else if constexpr (std::is_same_v<T, glm::mat4>) {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
        } else if constexpr (std::is_same_v<T, glm::vec3>) {
            glUniform3fv(loc, 1, glm::value_ptr(value));
        }
    }

private:
    GLuint mId;
};