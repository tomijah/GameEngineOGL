#include "MeshShader.h"

	const char *meshVert = MULTI_LINE_STRING(
		#version 330 core\n

		layout(location = 0) in vec3 position;
		layout(location = 1) in vec3 normal;
		layout(location = 2) in vec2 texCoords;

		out vec2 TexCoords;
		out vec3 FragPos;
		out vec3 Normal;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		

		void main()
		{
			gl_Position = projection * view * model * vec4(position, 1.0f);
			TexCoords = texCoords;
			FragPos = vec3(model * vec4(position, 1.0f));
			Normal = mat3(transpose(inverse(model))) * normal;
		}
	);

	const char *meshFrag = MULTI_LINE_STRING(
		#version 330 core\n

		in vec2 TexCoords;
		in vec3 FragPos;
		in vec3 Normal;
		out vec4 color;
		uniform sampler2D texture_diffuse1;
		uniform sampler2D texture_specular1;
		uniform vec3 viewPos;
		float specularStrength = 0.2f;

		void main()
		{
			vec3 norm = normalize(Normal);
			vec3 lightColor = vec3(1, 1, 1);
			vec3 lightDir = normalize(vec3(2.0, 5.0, 5.0) - FragPos);

			//diffuse
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = diff * lightColor * vec3(texture(texture_diffuse1, TexCoords));

			//specular
			vec3 viewDir = normalize(viewPos - FragPos);
			vec3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			vec3 specular = specularStrength * spec * lightColor * vec3(texture(texture_specular1, TexCoords));

			color = vec4(diffuse + specular, 1.0);
		}
	);



	MeshShader::MeshShader()
	{
		AttachShader(meshVert, GL_VERTEX_SHADER);
		AttachShader(meshFrag, GL_FRAGMENT_SHADER);
		Link();
	}