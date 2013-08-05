#version 330 core

uniform sampler2D color_texture;
uniform int right;

in vec4 pass_Color;
in vec2 UV;

layout(location = 0) out vec4 out_Color;
void main(void)
{
    out_Color = pass_Color;
}