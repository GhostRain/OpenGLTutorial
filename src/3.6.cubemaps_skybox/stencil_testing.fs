#version 330 core
out vec4 color;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I,Normal);
    color = texture(skybox, R);
}