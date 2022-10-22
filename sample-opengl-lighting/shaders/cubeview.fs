#version 330 core

struct Material {
    // defines the color of the object's surface under lighting
    vec3 ambient;
    // defines the color of the object's surface under diffuse lighting
    vec3 diffuse;
    // defines the color of of the specular highlight on the object's surface
    vec3 specular;
    // defines the scattering/radius of the specular highlight
    float shininess;
};

struct Light {
    // defines the strength of the light
    vec3 ambient;
    // defines the color of the light
    vec3 diffuse;
    // defines the shine of the light
    vec3 specular;
    vec3 position;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform Material material;
uniform Light light;

void main() {
    // Phong lighting model consists of 3 components: ambient, diffuse, specular
    // ambient lighting defines the strength of the light source, which affects the color of the object
    vec3 ambient = light.ambient * material.ambient;

    // diffuse lighting simulates the directional impact a light source has on an object
    // in order to calculate the diffuse lighting, we need the following:
    // 1. Normal vector: vector that is perpendicular to the fragment
    // 2. Light direction: difference between the light source's position and the fragment position.
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diffuseImpact = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diffuseImpact * material.diffuse);

    // specular lighting simulates the bright spot of a light that appears on shiny object.
    vec3 viewDir = normalize(-FragPos);
    // reflect function expects the light direction coming from the light source. Since the previously calculated light direction is from the perspective of the fragment, we negate the value.
    vec3 reflectDir = reflect(-lightDir, normal);
    // raising the dot product value by a value (e.g. 32) signifies the shininess value of the highlight. The higher the shininess value, the more prominent the highlight is reflected on the fragment.
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (specularImpact * material.specular);

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}