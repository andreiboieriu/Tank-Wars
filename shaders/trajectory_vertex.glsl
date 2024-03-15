#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out vec3 position;
out vec2 texCoords;
out vec3 normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    // translate position and normal to world space
    position = vec3(model * vec4(aPosition, 1.0));
    texCoords = aTexCoords;
    normal = aNormal;

    gl_Position = projection * view * vec4(position, 1.0);
}