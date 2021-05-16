#version 450

layout (location = 0) out vec2 outUV;

void main()
{
	outUV = vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1);
	gl_Position = vec4(outUV * 2.0f - 1.0f, 0.0f, 1.0f);
}