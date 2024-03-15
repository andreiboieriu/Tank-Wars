#version 460 core
layout (location = 0) in vec4 vertex;

out vec2 textureCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    textureCoords = vertex.zw;

    gl_Position = projection * model * vec4(vertex.x, vertex.y, 0.0f, 1.0);
}