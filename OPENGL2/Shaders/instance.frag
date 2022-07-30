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
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
uniform DirLight dirLight;
uniform Material material;
#define NR_POINT_LIGHTS 1  
uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec3 normal;
in vec3 cameraPos;
in vec3 fragPos;

in vec2 diffuseTexCoords;
out vec4 outColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, diffuseTexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, diffuseTexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, diffuseTexCoords));
    return vec3(ambient+diffuse);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, diffuseTexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, diffuseTexCoords));
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse);
} 


void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - fragPos);
    
    vec3 result;
//
//    // phase 1: Directional lighting
    result = CalcDirLight(dirLight, norm, viewDir);
//    // phase 2: Point lights
   for(int i = 0; i < NR_POINT_LIGHTS; i++)
     result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);   

   outColor = vec4(result,1);
}

