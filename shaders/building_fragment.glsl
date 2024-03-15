#version 460 core

out vec4 fragColor;

in vec3 position;
in vec2 texCoords;
in vec3 normal;
in vec3 tangentViewPosition;
in vec3 tangentPosition;
in vec3 lightDirection;

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

// uniform vec3 viewPosition;
uniform material_t material;
uniform light_t light;

void main()
{
    // ambient component
    vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

    // diffuse component
    vec3 diffuse = light.diffuse *
                   max(dot(normal, lightDirection), 0.0) *
                   texture(material.diffuse, texCoords).rgb;

    // specular component
    vec3 viewDirection = normalize(tangentViewPosition - tangentPosition);
    // vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 halfwayDirection = normalize(viewDirection + lightDirection);
    float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;

    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, 1.0);
} 