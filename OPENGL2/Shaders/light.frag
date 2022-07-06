#version 330 
out vec4 outColor;
in vec2 fragTexCoord;
uniform sampler2D uTexture;
void main()
{
	outColor = vec4(1.0,0.8,0.6,1.0);
}