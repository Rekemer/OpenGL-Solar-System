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
	float displacementX =  sin(vertPos.x * 3)*0.5 + 1;
	float displacement =   sin(vertPos.x * 3)*0.5 + 1;
	vec3 dipls = vec3(aNormal.x *displacementX,aNormal.y *  displacement, aNormal.z *  displacement);
	vec3 newPosition = vertPos + dipls;

	float t = clamp( inverseLerp(0.2,0.45,texcoord.x), 0,1);
	float t1 = clamp( inverseLerp(0.8,0.1,texcoord.y), 0,1);
	vec4 pos = vec4(newPosition  ,1.0);

	
	//pos.x = sin(pos.x * 10);

	

	gl_Position =projMatrix *viewMatrix* worldMatrix * pos;
	normal = aNormal;
	

	
	fragPos = vec3(worldMatrix * pos);

}


