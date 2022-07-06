#version 330  
layout(location = 0) in vec3 worldPos;

uniform mat4 worldMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
void main()
{
	vec4 pos = vec4(worldPos,1.0);
	gl_Position =projMatrix *viewMatrix* worldMatrix * pos;
}
