#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    // we need to tranform the normal vectors to view space coordinates, since all calculation in the fragment shader is done in the view space.
    // Note: for the purpose of experiment, inverse calculation is done on the shader (GPU), but inverse calculation is expensive, so it's better to do
    // the calculation on the CPU and pass the result to the GPU.
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    // Transform world-space light position to view-space light position
    LightPos = vec3(view * vec4(lightPos, 1.0));
}