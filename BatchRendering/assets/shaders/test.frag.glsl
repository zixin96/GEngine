#version 450 core

layout (location = 0) out vec4 o_Color;

in vec4 fs_Color; 

void main()
{
	o_Color = fs_Color;
}