#version 330 
out vec4 outColor;
in vec2 fragTexCoord;
uniform sampler2D uTexture;
void main()
{
	outColor = vec4(0.2,0.8,0.5,1.0);
	outColor = texture(uTexture,fragTexCoord);
}