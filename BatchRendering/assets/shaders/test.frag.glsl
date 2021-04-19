#version 450 core

layout (location = 0) out vec4 o_Color;

in vec4 fs_Color; 
in vec2 fs_TexCoord;
in float fs_TexIndex;

uniform sampler2D u_Textures[2];

void main()
{
	int index = int(fs_TexIndex);
	o_Color = texture(u_Textures[index], fs_TexCoord);
}