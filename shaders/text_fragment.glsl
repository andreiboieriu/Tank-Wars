#version 460 core
in vec2 textureCoords;
out vec4 color;

uniform vec3 spriteColor;
uniform sampler2D image;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(image, textureCoords).r);

    color = vec4(spriteColor, 1.0) * sampled;
}  