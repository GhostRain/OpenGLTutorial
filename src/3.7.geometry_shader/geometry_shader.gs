#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{
    gl_Position = position + vec4(-0.2f, -0.2f, 0.0f, 0.0f);// 1:���½�
    EmitVertex();
    gl_Position = position + vec4( 0.2f, -0.2f, 0.0f, 0.0f);// 2:���½�
    EmitVertex();
    gl_Position = position + vec4(-0.2f,  0.2f, 0.0f, 0.0f);// 3:����
    EmitVertex();
    gl_Position = position + vec4( 0.2f,  0.2f, 0.0f, 0.0f);// 4:����
    EmitVertex();
    gl_Position = position + vec4( 0.0f,  0.4f, 0.0f, 0.0f);// 5:�ݶ�
    EmitVertex();
    EndPrimitive();
}

void main() {   
	fColor = gs_in[0].color;
    build_house(gl_in[0].gl_Position);
}