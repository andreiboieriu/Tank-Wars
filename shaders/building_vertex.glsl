#version 460 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out vec3 position;
out vec2 texCoords;

// out vec3 tangentLightPosition;
out vec3 tangentViewPosition;
out vec3 tangentPosition;
out vec3 normal;
out vec3 lightDirection;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

// uniform vec3 lightPosition;
uniform vec3 viewPosition;

struct light_t {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct material_t {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D height;
    float shininess;
};

uniform material_t material;

uniform light_t light;

void main()
{
    // translate position and normal to world space
    position = vec3(model * vec4(aPosition, 1.0));
    texCoords = aTexCoords;

    // get normal from normal map
    normal = texture(material.height, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // compute tangents
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);

    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));

    tangentViewPosition = TBN * viewPosition;
    tangentPosition = TBN * position;

    lightDirection = normalize(TBN * -light.direction);

    gl_Position = projection * view * vec4(position, 1.0);
}