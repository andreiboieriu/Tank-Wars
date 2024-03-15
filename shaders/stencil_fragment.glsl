#version 460 core

out vec4 fragColor;

uniform int hp;

void main() {
    if (hp == 3)
        fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    else if (hp == 2)
        fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    else if (hp == 1)
        fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    else
        fragColor = vec4(0.f);
}