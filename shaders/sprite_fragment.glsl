#version 460 core

out vec4 fragColor;

in vec2 textureCoords;

uniform sampler2D image;
uniform float opacity;

void main()
{
    vec4 texColor = texture(image, textureCoords);

    // texColor.a = alpha;

    // if (texColor.a < 0.1)
    //     discard;

    fragColor = vec4(vec3(texColor), opacity);
} 