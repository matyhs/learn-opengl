#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);

    vec3 result = CalcDirectionalLight(directionalLight, normal, viewDir);
    result += CalcPointLight(pointLight, normal, viewDir);
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

    vec3 lightDir = normalize(-light.direction);
    float diffuseImpact = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseImpact * vec3(texture(material.texture_diffuse1, TexCoords));
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * specularImpact * vec3(texture(material.texture_specular1, TexCoords));

    return ambient + diffuse;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

    vec3 lightDir = normalize(light.position - FragPos);
    float diffuseImpact = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseImpact * vec3(texture(material.texture_diffuse1, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularImpact = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * specularImpact * vec3(texture(material.texture_diffuse1, TexCoords));

    float dist = length(light.position - FragPos);
    float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}