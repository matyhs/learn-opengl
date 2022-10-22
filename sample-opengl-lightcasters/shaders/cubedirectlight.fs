#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// when the light source is far away, the light rays are close to parallel to each other, which makes it seems like the light rays are all coming from same direction.
// This type of light source is called directional light. This type of light is great for lighting up the whole scene. A good example is the sun.
struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    vec3 lightDir = normalize(-light.direction);
    float diffuseImpact = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseImpact * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 materialSpecular = vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * specularImpact * materialSpecular;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}