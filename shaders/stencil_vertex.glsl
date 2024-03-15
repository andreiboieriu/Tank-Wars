#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform int hp;

void main() {
    gl_Position = projection * view * model * vec4(aPosition + aNormal * 0.02, 1.0);
}