#version 330 core
#define colorcode 1
in vec2 UV; 
out vec4 color;
 
uniform sampler2D renderedTexture1;
uniform sampler2D renderedTexture2;
 
void main(){
	vec4 leftFrag = texture(renderedTexture2, UV);
	vec4 rightFrag = texture(renderedTexture1, UV);

#if colorcode
	leftFrag = vec4( leftFrag.r, leftFrag.g, 0.0, 1.0 );
	rightFrag = vec4( 0.0, 0.0, rightFrag.b, 1.0 );
#else
	leftFrag = vec4(0.0, leftFrag.g, leftFrag.b, 1.0);
	rightFrag = vec4(rightFrag.r, 0.0, 0.0, 1.0); 
#endif
	
	color = vec4(leftFrag.rgb + rightFrag.rgb, 1.0); 
}