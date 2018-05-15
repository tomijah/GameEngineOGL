#version 330 core
out vec4 color;
uniform sampler2D texture1;
in vec2 UV;
void main()
{
	float depthValue = texture(texture1, UV).r;
	color = vec4(vec3(depthValue), 1.0);
}