#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

out vec4 color;

const int MAX_POINT_LIGHTS = 10;

struct Light
{
	vec3 color;
	float ambiantIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D texture2D;
uniform Material material;
uniform vec3 eyePosition;

vec4 ComputeLightFromDirection(Light light, vec3 direction)
{
	vec4 ambiantColor = vec4(light.color, 1.0f) * light.ambiantIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);

	vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0.0f);

	if (diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPosition);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	return ambiantColor + diffuseColor + specularColor;
}

vec4 ComputeDirectionalLight()
{
	return ComputeLightFromDirection(directionalLight.base, directionalLight.direction);
}

vec4 ComputePointLight()
{
	vec4 totalColor = vec4(0.0f);
	for(int i; i < pointLightCount; i++)
	{
		PointLight currentLight = pointLights[i];
		vec3 direction = FragPosition - currentLight.position;
		float distanceToLight = length(direction);
		if (distanceToLight > 0)
		{
			direction = direction / distanceToLight;
		}
		vec4 color = ComputeLightFromDirection(currentLight.base, direction);
		float attenuation = currentLight.quadratic * distanceToLight * distanceToLight
						  + currentLight.linear * distanceToLight
						  + currentLight.constant;
		totalColor += color / attenuation;
	}
	return totalColor;
}

void main()
{
	vec4 finalColor = ComputeDirectionalLight();
	finalColor += ComputePointLight();
	color = texture(texture2D, TexCoord) * finalColor;
}