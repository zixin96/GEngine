#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec3 fs_Normal;
out vec3 fs_Pos;
out vec2 fs_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
	fs_Normal = a_Normal;
	fs_TexCoord = a_TexCoord;
	fs_Pos = a_Position;
}