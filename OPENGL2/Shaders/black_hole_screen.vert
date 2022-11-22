#version 330  


layout(location = 0) in vec3 vertPos;
layout (location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoord;


float lerp(float a,float b,float t)
{
float c = (1-t)*a +t*b;
return c;
}

float inverseLerp(float a,float b,float v){
return (v-a)/(b-a);
}


out vec3 normal;
out vec3 fragPos;
out vec2 diffuseTexCoords;
uniform mat4 worldMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
void main()
{
    vec2 texcoord = vec2(texCoord.x, texCoord.y);
	diffuseTexCoords = texcoord;
	normal = aNormal;
	vec4 pos = vec4(vertPos  ,1.0);
	fragPos = vec3(projMatrix *viewMatrix* worldMatrix * pos);
	gl_Position = vec4(vertPos.x, vertPos.y, 0.0, 1.0); 
    diffuseTexCoords = texCoord;

}


