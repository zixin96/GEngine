#version 460 core

layout (location = 0) out vec4 o_Color;

uniform vec4 u_LightColor;

void main()
{
    o_Color = u_LightColor;
}