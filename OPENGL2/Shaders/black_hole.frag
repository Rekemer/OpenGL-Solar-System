#version 330 
 float PI =3.14159265359;

float random (vec2 st);
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

struct Light {
    vec3 position;
	vec3 direction;
	float cutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 


struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  


struct SpotLight{
    vec3 position;
    float cutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;

};
uniform SpotLight spotLight;
vec3 CalcSpotLight( SpotLight spotLight,vec3  normal, vec3 fragPos, vec3 viewDir); 

uniform Light light;
uniform Material material;
uniform float time;
uniform float radius;
uniform vec3 dist;
uniform vec3 cameraPos;

in vec2 diffuseTexCoords;
layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 BrightColor;

in vec3 normal;
in vec3 lightPosition;
in vec3 fragPos;

void main()
{
    outColor = vec4(1,1,1,1);
}
