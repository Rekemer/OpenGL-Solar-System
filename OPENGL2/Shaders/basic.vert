#version 330  


layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNormal;
out vec2 fragTexCoord;
out vec3 normal;
out vec3 lightPosition;
out vec3 fragPos;

uniform mat4 worldMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
void main()
{
    vec2 texcoord = vec2(texCoord.x, 1.0-texCoord.y);
	fragTexCoord = texcoord;
	vec4 pos = vec4(vertPos,1.0);
	gl_Position =projMatrix *viewMatrix* worldMatrix * pos;
	normal = aNormal;
	fragPos = vec3(worldMatrix * pos);

}
