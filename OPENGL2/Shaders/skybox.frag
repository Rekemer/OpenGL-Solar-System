#version 330 

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
   // sampler2D texture_diffuse3;
    sampler2D texture_specular1;
   // sampler2D texture_specular2;
	//sampler2D diffuse;
    vec3 specular;
    float shininess;
};


uniform Material material;
uniform float time;
uniform vec3 cameraPos;

in vec2 diffuseTexCoords;
out vec4 outColor;


in vec3 fragPos;



void main()
{
   outColor = texture(material.texture_diffuse1, diffuseTexCoords);
}

