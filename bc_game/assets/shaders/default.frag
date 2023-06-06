#version 430 core

out vec4 FragColor;

in vec4  Color;
in vec2  UV;
in float Palatte_idx;
in float Texture_idx;
in float B_use_sample;
in float Text_sample_idx;
in vec2 Sample_uv;

uniform vec4 clut[8][4];
uniform sampler2D textures[32];

void main()
{
    float color_id = texture(textures[ int(Texture_idx) ], UV).r;	
	int   idx      = 0;

    if(B_use_sample > 0.1f)
	{
	    color_id = texture(textures[int(Text_sample_idx)], Sample_uv).r;
		idx      = int(color_id * 4);

		if(color_id > 0.0f)
		{
		    FragColor = clut[ int(Palatte_idx) ][ idx ] * Color;
		}
	}
	else
	{
	    idx = int(color_id * 4);
		if(idx > 3)
		{
		    FragColor = Color;
		}
		else
		{
		    FragColor = clut[ int(Palatte_idx) ][ idx ] * Color;
		}
	}	
}