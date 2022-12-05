#version 330

uniform samplerCube depthMap;
in vec3 TexCoords;
out vec4 outColor;
void main()
{
	//outColor = vec4(1.,1.,0,1);
	outColor = texture(depthMap,TexCoords);
}