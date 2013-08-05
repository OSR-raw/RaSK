#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in vec2 in_UV;

out vec4 pass_Color;
out vec2 UV;

void main(void)
{
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
     pass_Color = in_Color;
	 UV = in_UV;
}