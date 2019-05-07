#version 330 core
in vec3 Normal;
in vec3 FragPos;  
out vec4 color;

uniform vec3 col;
uniform vec3 lightPos;

void main()
{
	if(lightPos.y <= -100.0)
	{
		color = vec4(col, 1.0);
		return;
	}

	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 norm = normalize(Normal);
  	
    // diffuse 
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = diffuse * col;
	color = vec4(result, 1.0);
}
