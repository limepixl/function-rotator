#version 120
varying out vec4 color;

uniform vec3 col;

void main()
{
	color = vec4(col, 1.0);
}
