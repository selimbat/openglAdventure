#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

out vec4 color;

struct DirectionalLight
{
	vec3 color;
	float ambiantIntencity;
	vec3 direction;
	float diffuseIntencity;
};

struct Material
{
	float specularIntencity;
	float shininess;
};

uniform sampler2D texture2D;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 eyePosition;

void main()
{
	vec4 ambiantColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambiantIntencity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);

	vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntencity * diffuseFactor;

	vec4 specularColor = vec4(0.0f);

	if (diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPosition);
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(directionalLight.color * material.specularIntencity * specularFactor, 1.0f);
		}
	}
	color = texture(texture2D, TexCoord) * (ambiantColor + diffuseColor + specularColor);
}