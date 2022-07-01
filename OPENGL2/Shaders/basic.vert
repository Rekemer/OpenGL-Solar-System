#version 330  
layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec2 texCoord;

out vec2 fragTexCoord;
void main()
{
	fragTexCoord = texCoord;
	vec4 pos = vec4(worldPos,1.0);
	gl_Position = pos;
}
