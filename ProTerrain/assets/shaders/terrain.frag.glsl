#version 460 core

layout (location = 0) out vec4 o_Color;

in vec3 fs_Normal;
in vec2 fs_TexCoord;
in float fs_Height;
in vec3 fs_Pos;

uniform float u_Changing;
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;

void main()
{
    vec4 terrainColor = vec4(0.0, 0.0, 0.0, 0.0);
	if(fs_Height < 0.3)
	{
        terrainColor = vec4(0.2212, 0.3564, 0.4150, 1.0);
	}
	else if(fs_Height < 0.4)
	{
        terrainColor = vec4(0.20, 0.39, 0.48, 1.0);
	}
	else if(fs_Height < 0.45)
	{
        terrainColor = vec4(1.0, 0.97, 0.37, 1.0);
	}
	else if(fs_Height < 0.55)
	{
        terrainColor = vec4(0.20, 0.63, 0.14, 1.0);
	}
	else if(fs_Height < 0.6)
	{
        terrainColor = vec4(0.08, 0.69, 0.0, 1.0);
	}
	else if(fs_Height < 0.7)
	{
        terrainColor = vec4(0.41, 0.32, 0.18, 1.0);
	}
	else if(fs_Height < 0.9)
	{
        terrainColor = vec4(0.40, 0.29, 0.10, 1.0);
	}
	else {
		terrainColor = mix(vec4(1.0, 1.0, 1.0, 1.0), vec4(0.40, 0.29, 0.10, 1.0), u_Changing);
	}
	vec3 norm = normalize(fs_Normal);
	vec3 lightDir = normalize(u_LightPos - fs_Pos);
	vec3 diffuse =  max(dot(norm, lightDir), 0.0) * u_LightColor.xyz;

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightColor.xyz;

	o_Color = terrainColor * vec4(ambient + diffuse, 1.0);
}