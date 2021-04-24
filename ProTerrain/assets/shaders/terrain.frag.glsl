#version 450 core

layout (location = 0) out vec4 o_Color;

in vec3 fs_Normal;
in vec2 fs_TexCoord;

void main()
{
	o_Color = vec4(0.8, 0.1, 0.3, 1.0);
}