#version 130
in vec3 aPos;
in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Normal = aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
