#version 330  
layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec2 texCoord;

out vec2 fragTexCoord;
uniform mat4 worldMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
void main()
{
    vec2 texcoord = vec2(texCoord.x, 1.0-texCoord.y);
	fragTexCoord = texcoord;
	vec4 pos = vec4(worldPos,1.0);
	gl_Position =projMatrix *viewMatrix* worldMatrix * pos;
}
