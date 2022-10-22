#version 330 core
out vec4 FragColor;

in vec3 pColor;
in vec2 pTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float textureVisibility;

void main()
{
	FragColor = mix(texture(texture1, pTexCoord), texture(texture2, vec2(1.0 - pTexCoord.x, pTexCoord.y)), textureVisibility) * vec4(pColor, 1.0);
}

