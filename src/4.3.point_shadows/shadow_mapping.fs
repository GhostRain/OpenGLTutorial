#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
	float lengthDis = length(FragPos.xyz - lightPos);
	lengthDis = lengthDis/far_plane;
	gl_FragDepth = lengthDis;
}