#version 460 core
out vec4 color;

uniform vec3 col;

void main()
{
	color = vec4(col, 1.0);
}
