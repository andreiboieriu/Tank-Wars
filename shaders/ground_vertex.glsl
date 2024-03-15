#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aOffset;

out vec3 position;
out vec2 texCoords;

// out vec3 tangentLightPosition;
out vec3 tangentViewPosition;
out vec3 tangentPosition;
out vec3 T;
out vec3 B;
out vec3 N;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

// uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
    // translate position and normal to world space
    position = vec3(model * vec4(aPosition + aOffset, 1.0));
    texCoords = aTexCoords;

    // compute tangents
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    T = normalize(normalMatrix * aTangent);
    N = normalize(normalMatrix * aNormal);

    T = normalize(T - dot(T, N) * N);
    B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));

    tangentViewPosition = TBN * viewPosition;
    tangentPosition = TBN * position;

    gl_Position = projection * view * vec4(position, 1.0);
}