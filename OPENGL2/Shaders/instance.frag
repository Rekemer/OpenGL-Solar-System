#version 330 
struct Material
{
    sampler2D texture_diffuse1;
   // sampler2D texture_diffuse2;
   // sampler2D texture_diffuse3;
    sampler2D texture_specular1;
   // sampler2D texture_specular2;
	//sampler2D diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;
in vec2 diffuseTexCoords;
out vec4 outColor;
void main()
{
   outColor = vec4(1,1,1,1);
}