#version 460 core

layout (location = 0) out vec4 o_Color;

in vec3 fs_Normal;
in vec2 fs_TexCoord;
in vec3 fs_Pos;

uniform float u_Changing;

void main()
{
	if(fs_Pos.y < 0.3)
	{
        o_Color = vec4(0.2212, 0.3564, 0.4150, 1.0);
	}
	else if(fs_Pos.y < 0.4)
	{
        o_Color = vec4(0.20, 0.39, 0.48, 1.0);
	}
	else if(fs_Pos.y < 0.45)
	{
        o_Color = vec4(1.0, 0.97, 0.37, 1.0);
	}
	else if(fs_Pos.y < 0.55)
	{
        o_Color = vec4(0.20, 0.63, 0.14, 1.0);
	}
	else if(fs_Pos.y < 0.6)
	{
        o_Color = vec4(0.08, 0.69, 0.0, 1.0);
	}
	else if(fs_Pos.y < 0.7)
	{
        o_Color = vec4(0.41, 0.32, 0.18, 1.0);
	}
	else if(fs_Pos.y < 0.9)
	{
        o_Color = vec4(0.40, 0.29, 0.10, 1.0);
	}
	else {
		o_Color = mix(vec4(1.0, 1.0, 1.0, 1.0), vec4(0.40, 0.29, 0.10, 1.0), u_Changing);
	}
}