#version 330 core

uniform sampler2D color_texture;

in vec4 pass_Color;
in vec2 UV;

out vec4 out_Color;

void main(void)
{
    //out_Color = pass_Color;// + UV.xyxx;
    out_Color = pass_Color;//texture( color_texture, UV );
}