#version 120
in vec3 Normal;
in vec3 FragPos;
varying out vec4 color;

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

	// Diffuse lighting
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(normalize(Normal), lightDir), 0.0);

	if(lightPos.y < -400.0)
		color = vec4(col, 0.2);
	else
		color = vec4(col * diff, 1.0);
}
