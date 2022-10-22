#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// a point light is a light source that is positioned somewhere in the world, and shoots light rays in all direction.
// Unlike directional light, the light rays fade out over distance.
// Examples are light bulbs and torches.
struct Light {
    vec3 position;
    // light components
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // luminosity attenuation properties
    // luminosity attenuation is the intensity of light over disntace.
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform Light light;

void main() {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diffuseImpact = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diffuseImpact * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * specularImpact * vec3(texture(material.specular, TexCoords));

    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}