#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture1;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

float specularStrength = 0.5f;

void main()
{
	float ambientStrength = 0.05f;
    vec3 ambient = ambientStrength * lightColor;
	
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	//∞Î≥ÃœÚ¡ø
	vec3 halfwayDir = normalize(lightDir + viewDir);
	//Phong
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	//Blinn-Phong
	float spec = pow(max(dot(halfwayDir, Normal), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
    vec4 result = vec4((ambient + diffuse + specular),1.0);

    FragColor = result * texture(texture1, TexCoords);
}