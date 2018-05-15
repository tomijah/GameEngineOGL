#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

out vec4 color;
uniform vec3 viewPos;
uniform vec3 lightPos;

struct Material {
	float shininess;
	vec3 color;
	bool hasTexture;
	bool hasSpecularMap;
	bool applyLights;
};

uniform Material material;

uniform sampler2D diffuseTexture;
uniform sampler2D specularMap;
uniform sampler2D shadowMap;

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	bool apply;
};

uniform DirLight dirLight;

struct PointLight {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	bool apply;
};

#define NR_POINT_LIGHTS 16

uniform PointLight pointLights[NR_POINT_LIGHTS];

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.001;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /=9.0;

	// shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
	 if(projCoords.z > 1.0)
        shadow = 0.0;
    
    return shadow * 0.5;
}  

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 diffColor = material.color;
	if (material.hasTexture) {
		diffColor = vec3(texture(diffuseTexture, TexCoords));
	}

	vec3 lightDir = normalize(-light.direction);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading

	vec3 specular = vec3(material.shininess);
	if(material.hasSpecularMap){
		vec3 reflectDir = reflect(-lightDir, normal); 
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		specular = spec * light.diffuse;
		specular *= vec3(texture(specularMap, TexCoords)) * 2;
	}

	// Combine results
	vec3 ambient = light.ambient * diffColor;
	vec3 diffuse = light.diffuse * diff * diffColor;

	float shadow = ShadowCalculation(FragPosLightSpace);  
	if(shadow > 0.0){
		return (1.0 - shadow) * (ambient + diffuse);
	}
	return (1.0 - shadow) * (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 diffColor = material.color;
	if (material.hasTexture) {
		diffColor = vec3(texture(diffuseTexture, TexCoords));
	}

	vec3 lightDir = normalize(light.position - fragPos);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 specular = vec3(material.shininess);
	if(material.hasSpecularMap){
		vec3 reflectDir = reflect(-lightDir, normal); 
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		specular = spec * light.diffuse;  
		specular *= vec3(texture(specularMap, TexCoords)) * 2;
	}
	// Attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist +
		light.quadratic * (dist * dist));
	// Combine results
	vec3 ambient = light.ambient  * diffColor;
	vec3 diffuse = light.diffuse  * diff * diffColor;
    ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0, 0, 0);
	if(!material.applyLights)
	{
		color = vec4(material.color, 1.0);
	}
	else{
		if (dirLight.apply) {
			result += CalcDirLight(dirLight, norm, viewDir);
		}
				
		for (int i = 0; i < NR_POINT_LIGHTS; i++) {
			if (pointLights[i].apply) {
				result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
			}
		}

		color = vec4(result, 1.0);
	}
}