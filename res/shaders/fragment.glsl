#version 460 core
in vec3 Normal;
out vec4 color;

uniform vec3 col;

void main()
{
	color = vec4(Normal, 1.0);
}
