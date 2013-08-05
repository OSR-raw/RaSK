#version 330 core

uniform mat4 pvm;
layout(location = 0) in vec3 in_Position;

out vec4 pass_Color;
out vec4 glPos;

void main(){
	gl_Position = pvm * vec4(in_Position, 1.0);
    pass_Color = vec4((in_Position + 1)/2.0, 1.0);
	glPos = gl_Position;
}

