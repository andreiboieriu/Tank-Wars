#version 460 core

out vec4 fragColor;

in vec3 position;
in vec2 texCoords;
in vec3 normal;

struct material_t {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D height;
    float shininess;
};

struct light_t {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPosition;
uniform material_t material;
uniform light_t light;
uniform float opacity;

void main()
{
    vec3 lightDirection = normalize(-light.direction);

    vec3 _normal = normalize(normal);

    // ambient component
    vec3 ambient = light.ambient * vec3(0.0f, 1.0f, 0.0f);

    // diffuse component
    vec3 diffuse = light.diffuse *
                   max(dot(_normal, lightDirection), 0.0) *
                   vec3(0.0f, 1.0f, 0.0f);

    // specular component
    vec3 viewDirection = normalize(viewPosition - position);
    // vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 halfwayDirection = normalize(viewDirection + lightDirection);
    float spec = pow(max(dot(_normal, halfwayDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(0.0f, 1.0f, 0.0f);

    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, opacity);
} 