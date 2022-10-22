#version 330 core

struct Material {
    // use diffuse map to define the diffuse and ambient color based on the different material used in the object
    sampler2D diffuse;
    // use diffuse map to define the highlight on material used in the object
    sampler2D specular;
    // use emission map to define light sources in object
    sampler2D emission;
    vec3 emissionColor;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;

void main() {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diffuseImpact = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseImpact * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 materialSpecular = vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * specularImpact * materialSpecular;

    vec3 emission = vec3(0.0);
    if (materialSpecular == vec3(0.0) && diffuse != vec3(0.0))
    {
        emission = material.emissionColor * vec3(texture(material.emission, TexCoords));
    }

    FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}
