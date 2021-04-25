#version 460 core

layout (location = 0) out vec4 o_Color;

in vec4 fs_Color; 
in vec2 fs_TexCoord;
in float fs_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = fs_Color;
    switch(int(fs_TexIndex))
    {
        case 0: texColor *= texture(u_Textures[0], fs_TexCoord ); break;
        case 1: texColor *= texture(u_Textures[1], fs_TexCoord ); break;
        case 2: texColor *= texture(u_Textures[2], fs_TexCoord ); break;
        case 3: texColor *= texture(u_Textures[3], fs_TexCoord ); break;
        case 4: texColor *= texture(u_Textures[4], fs_TexCoord ); break;
        case 5: texColor *= texture(u_Textures[5], fs_TexCoord ); break;
        case 6: texColor *= texture(u_Textures[6], fs_TexCoord ); break;
        case 7: texColor *= texture(u_Textures[7], fs_TexCoord ); break;
        case 8: texColor *= texture(u_Textures[8], fs_TexCoord ); break;
        case 9: texColor *= texture(u_Textures[9], fs_TexCoord ); break;
        case 10: texColor *= texture(u_Textures[10], fs_TexCoord ); break;
        case 11: texColor *= texture(u_Textures[11], fs_TexCoord ); break;
        case 12: texColor *= texture(u_Textures[12], fs_TexCoord ); break;
        case 13: texColor *= texture(u_Textures[13], fs_TexCoord ); break;
        case 14: texColor *= texture(u_Textures[14], fs_TexCoord ); break;
        case 15: texColor *= texture(u_Textures[15], fs_TexCoord ); break;
        case 16: texColor *= texture(u_Textures[16], fs_TexCoord ); break;
        case 17: texColor *= texture(u_Textures[17], fs_TexCoord ); break;
        case 18: texColor *= texture(u_Textures[18], fs_TexCoord ); break;
        case 19: texColor *= texture(u_Textures[19], fs_TexCoord ); break;
        case 20: texColor *= texture(u_Textures[20], fs_TexCoord ); break;
        case 21: texColor *= texture(u_Textures[21], fs_TexCoord ); break;
        case 22: texColor *= texture(u_Textures[22], fs_TexCoord ); break;
        case 23: texColor *= texture(u_Textures[23], fs_TexCoord ); break;
        case 24: texColor *= texture(u_Textures[24], fs_TexCoord ); break;
        case 25: texColor *= texture(u_Textures[25], fs_TexCoord ); break;
        case 26: texColor *= texture(u_Textures[26], fs_TexCoord ); break;
        case 27: texColor *= texture(u_Textures[27], fs_TexCoord ); break;
        case 28: texColor *= texture(u_Textures[28], fs_TexCoord ); break;
        case 29: texColor *= texture(u_Textures[29], fs_TexCoord ); break;
        case 30: texColor *= texture(u_Textures[30], fs_TexCoord ); break;
        case 31: texColor *= texture(u_Textures[31], fs_TexCoord ); break;
    }
	o_Color = texColor;
}