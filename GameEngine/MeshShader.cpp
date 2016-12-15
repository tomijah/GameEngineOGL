#include "MeshShader.h"

	const char *meshVert = MULTI_LINE_STRING(
		#version 330 core\n

		layout(location = 0) in vec3 position;
		layout(location = 1) in vec3 normal;
		layout(location = 2) in vec2 uv;

		out vec3 FragPos;
		out vec3 Normal;
		out vec2 TexCoords;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			vec4 positionModel = model * vec4(position, 1.0f);
			gl_Position = projection * view * positionModel;
			FragPos = vec3(positionModel);
			Normal = mat3(transpose(inverse(model))) * normal;
			TexCoords = uv;
		}
	);

	const char *meshFrag = MULTI_LINE_STRING(
		#version 330 core\n

		in vec3 FragPos;
		in vec3 Normal;
		in vec2 TexCoords;

		out vec4 color;
		uniform vec3 viewPos;

		struct Material {
			float shininess;
			vec3 color;
			bool hasTexture;
		};

		uniform Material material;

		uniform sampler2D diffuseTexture;

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

		};\n
		#define NR_POINT_LIGHTS 16\n
		uniform PointLight pointLights[NR_POINT_LIGHTS];

		vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
		{
			vec3 diffColor = material.color;
			if (material.hasTexture) {
				diffColor = vec3(texture(diffuseTexture, TexCoords));
			}

			vec3 lightDir = normalize(-light.direction);
			// Diffuse shading
			float diff = max(dot(normal, lightDir), 0.0);
			// Combine results
			vec3 ambient = light.ambient * diffColor;
			vec3 diffuse = light.diffuse * diff * diffColor;
			return (ambient + diffuse);
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
			// Attenuation
			float distance = length(light.position - fragPos);
			float attenuation = 1.0f / (light.constant + light.linear * distance +
				light.quadratic * (distance * distance));
			// Combine results
			vec3 ambient = light.ambient  * diffColor;
			vec3 diffuse = light.diffuse  * diff * diffColor;
			ambient *= attenuation;
			diffuse *= attenuation;
			return (ambient + diffuse);
		}

		void main()
		{
			vec3 norm = normalize(Normal);
			vec3 viewDir = normalize(viewPos - FragPos);

			vec3 result = vec3(0, 0, 0);
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
	);

	MeshShader::MeshShader()
	{
		AttachShader(meshVert, GL_VERTEX_SHADER);
		AttachShader(meshFrag, GL_FRAGMENT_SHADER);
		Link();
	}