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

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;

void main() {
    // Phong lighting model consists of 3 components: ambient, diffuse, specular
    // ambient lighting defines the strength of the light source, which affects the color of the object
    vec3 ambient = lightColor * material.ambient;

    // diffuse lighting simulates the directional impact a light source has on an object
    // in order to calculate the diffuse lighting, we need the following:
    // 1. Normal vector: vector that is perpendicular to the fragment
    // 2. Light direction: difference between the light source's position and the fragment position.
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffuseImpact = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightColor * (diffuseImpact * material.diffuse);

    // specular lighting simulates the bright spot of a light that appears on shiny object. 
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect function expects the light direction coming from the light source. Since the previously calculated light direction is from the perspective of the fragment, we negate the value.
    vec3 reflectDir = reflect(-lightDir, normal);
    // raising the dot product value by a value (e.g. 32) signifies the shininess value of the highlight. The higher the shininess value, the more prominent the highlight is reflected on the fragment.
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (specularImpact * material.specular);

    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}