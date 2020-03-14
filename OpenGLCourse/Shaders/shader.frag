#version 330

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

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float cutOffCos;	
};

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;
in vec4 directionalLightSpacePos;

out vec4 color;

const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D texture2D;
uniform sampler2D directionalShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];
uniform Material material;
uniform vec3 eyePosition;

float ComputeDirectionalShadowFactor(DirectionalLight light)
{
	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + vec3(0.5);

	float closest = texture(directionalShadowMap, projCoords.xy).r;
	float current = projCoords.z;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.direction);

	float bias = 0.005 * max(1 - dot(normal, lightDir), 0.1);

	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	float shadow = 0.0;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += current > pcfDepth + bias ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	if (current > 1.0)
	{
		shadow = 0.0;
	}

	return shadow;
}

float ComputeOmniShadowFactor(PointLight light, int shadowIndex)
{
	vec3 fragToLight = FragPosition - light.position;
	float current = length(fragToLight);

	float shadow = 0.0;
	float bias = 0.05;
	float smoothingRadius = 0.005;

	vec3 normalizedFragToLight = fragToLight;
	if (current > 0)
	{
		normalizedFragToLight = fragToLight / current;
	}
	vec3 ortho1 = vec3(0.0);
	if (dot(normalizedFragToLight, vec3(0.0, 1.0, 0.0)) < 1)
	{
		ortho1 = cross(normalizedFragToLight, vec3(0.0, 1.0, 0.0));
	}
	else
	{
		ortho1 = cross(normalizedFragToLight, vec3(0.0, 0.0, 1.0));
	}
	vec3 ortho2 = cross(ortho1, normalizedFragToLight);

	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
				float closest = texture(omniShadowMaps[shadowIndex].shadowMap, normalizedFragToLight + smoothingRadius * (i * ortho1 + j * ortho2)).r;
				closest *= omniShadowMaps[shadowIndex].farPlane;
				if (current - bias > closest)
				{
					shadow += 1;
				}
		}
	}

	shadow /= 9.0;

	return shadow;
}

vec4 ComputeLightFromDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambiantColor = vec4(light.color, 1.0f) * light.ambiantIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), direction), 0.0f);

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
	return ambiantColor + (1.0 - shadowFactor) * (diffuseColor + specularColor);
}

vec4 ComputeDirectionalLight()
{
	float shadowFactor = ComputeDirectionalShadowFactor(directionalLight);
	return ComputeLightFromDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

float PointLightAttenuationFunction(PointLight pointLight, float distance)
{
	return 1.0 / (pointLight.quadratic * pow(max(0, distance), 2.0));
}

vec4 ComputePointLight(PointLight pointLight, int shadowIndex)
{
	vec3 direction = FragPosition - pointLight.position;
	float distanceToLight = length(direction);
	if (distanceToLight > 0)
	{
		direction = direction / distanceToLight;
	}

	float shadowFactor = ComputeOmniShadowFactor(pointLight, shadowIndex);

	vec4 color = ComputeLightFromDirection(pointLight.base, direction, shadowFactor);
	float attenuation = min(1.0, PointLightAttenuationFunction(pointLight,
															   distanceToLight 
															   - pointLight.constant
															   + inversesqrt(pointLight.quadratic)));

	return (color * attenuation);
}

vec4 ComputePointLights()
{
	vec4 totalPointLightColor = vec4(0.0f);
	for(int i = 0; i < pointLightCount; i++)
	{
		totalPointLightColor += ComputePointLight(pointLights[i], i);
	}
	return totalPointLightColor;
}

vec4 ComputeSpotLight(SpotLight spotLight, int shadowIndex)
{
	vec3 directionToFrag = normalize(FragPosition - spotLight.base.position);
	float cosToFrag = dot(directionToFrag, spotLight.direction);
	bool isFragLit = cosToFrag > spotLight.cutOffCos;
	if (isFragLit)
	{
		float fadeFactor = (cosToFrag - spotLight.cutOffCos) / (1.0f - spotLight.cutOffCos);
		return fadeFactor * ComputePointLight(spotLight.base, shadowIndex);
	}
	return vec4(0.0f);
}

vec4 ComputeSpotLights()
{
	vec4 totalSpotLightColor = vec4(0.0f);
	for(int i = 0; i < spotLightCount; i++)
	{
		totalSpotLightColor += ComputeSpotLight(spotLights[i], i + pointLightCount);
	}
	return totalSpotLightColor;
}

void main()
{
	vec4 finalColor = ComputeDirectionalLight();
	finalColor += ComputePointLights();
	finalColor += ComputeSpotLights();
	color = texture(texture2D, TexCoord) * finalColor;
}