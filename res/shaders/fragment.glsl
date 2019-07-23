#version 130
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform vec3 col;
uniform vec3 lightPos;
uniform float planeCoord;
uniform int plane;	// 0 for x axis, 1 for y axis, 2 for z axis

void main()
{
	if(plane == 0 && FragPos.x > planeCoord)
	{
		if(FragPos.x < planeCoord + 0.01f)
		{
			color = vec4(1.0, 0.0, 1.0, 1.0);
			return;
		}
		discard;
	}
	else if(plane == 1 && FragPos.y > planeCoord)
	{
		if(FragPos.y < planeCoord + 0.01f)
		{
			color = vec4(1.0, 0.0, 1.0, 1.0);
			return;
		}
		discard;
	} else if(plane == 2 && FragPos.z > planeCoord)
	{
		if(FragPos.z < planeCoord + 0.01f)
		{
			color = vec4(1.0, 0.0, 1.0, 1.0);
			return;
		}
		discard;
	}

	color = vec4(col, 1.0);
}
