#version 330

in vec4 vCol;
in vec2 TexCoord;

out vec4 color;

struct DirectionalLight {
	vec3 color;
	float ambiantIntencity;
};

uniform sampler2D texture2D;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambiantColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambiantIntencity;

	color = texture(texture2D, TexCoord) * ambiantColor;
}