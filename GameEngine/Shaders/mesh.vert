#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{
	mat4 lightSpace = lightProjection * lightView;
	vec4 positionModel = model * vec4(position, 1.0f);
	gl_Position = projection * view * positionModel;
	FragPos = vec3(positionModel);
	FragPosLightSpace = lightSpace * vec4(FragPos, 1.0);
	Normal = mat3(transpose(inverse(model))) * normal;
	TexCoords = uv;
}