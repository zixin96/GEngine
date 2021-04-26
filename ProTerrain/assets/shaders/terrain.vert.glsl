#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec3 fs_Normal;
out float fs_Height;
out vec2 fs_TexCoord;
out vec3 fs_Pos;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
	fs_Normal = a_Normal;
	fs_TexCoord = a_TexCoord;
	fs_Height = a_Position.y;
	fs_Pos = vec3(u_Model * vec4(a_Position, 1.0));
}